#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define FILENAME "fileConditional.txt"

pthread_mutex_t mutex;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int is_data_ready = 0;

/// Thread functions
void *reader_thread(void *arg)
{
    FILE *fp;
    char buffer[128];

    while (1)
    {
        // Acquire mutex
        pthread_mutex_lock(&mutex);

        // Open file
        fp = fopen(FILENAME, "r");
        if (fp == NULL)
        {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        // Read from file
        fgets(buffer, sizeof(buffer), fp);
        printf("Read from file: %s", buffer);

        // Close file
        fclose(fp);

        is_data_ready = 1;
        pthread_cond_signal(&cond);

        // Release mutex
        pthread_mutex_unlock(&mutex);

        // Add delay
        sleep(1);
    }

    return NULL;
}

void *writer_thread(void *arg)
{
    FILE *fp;
    char buffer[128];

    while (1)
    {
        // Acquire mutex
        pthread_mutex_lock(&mutex);

        while (!is_data_ready)
        {
            // Wait for data to be ready
            pthread_cond_wait(&cond, &mutex);
        }

        // Open file
        fp = fopen(FILENAME, "w");
        if (fp == NULL)
        {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        // Write to file
        sprintf(buffer, "Random number: %d\n", rand() % 100);
        fputs(buffer, fp);
        printf("Wrote to file: %s", buffer);

        // Close file
        fclose(fp);

        is_data_ready = 0;

        // Release mutex
        pthread_mutex_unlock(&mutex);

        // Add delay
        sleep(1);
    }

    return NULL;
}

int main()
{
    int res;
    pthread_t t1, t2;

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    pthread_t reader_tid, writer_tid;
    res = pthread_create(&reader_tid, NULL, reader_thread, NULL);
    if (res != 0)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    res = pthread_create(&writer_tid, NULL, writer_thread, NULL);
    if (res != 0)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    // Wait for threads to finish
    pthread_join(reader_tid, NULL);
    pthread_join(writer_tid, NULL);

    // Cleanup
    pthread_mutex_destroy(&mutex);

    return 0;
}
