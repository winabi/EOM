#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define FILENAME "fileMunex.txt"

pthread_mutex_t mutex;

FILE *fp;         // File pointer
char buffer[128]; // Buffer to store data read from file

// Thread functions
void *reader_thread(void *arg)
{
    while (1)
    {
        // Acquire mutex
        printf("Reader: acquiring mutex...\n");
        pthread_mutex_lock(&mutex);
        printf("Reader: mutex acquired!\n");

        // Read from file
        fseek(fp, 0, SEEK_SET);
        fgets(buffer, 128, fp);
        printf("Read from file: %s", buffer);

        // Release mutex
        pthread_mutex_unlock(&mutex);
        printf("Reader: mutex released!\n");

        // Add delay
        usleep(500000);
    }

    return NULL;
}

void *writer_thread(void *arg)
{
    while (1)
    {
        // Acquire mutex
        printf("Writer: acquiring mutex...\n");
        pthread_mutex_lock(&mutex);
        printf("Writer: mutex acquired!\n");

        // Generate random number
        srand(time(NULL));
        int random_num = rand() % 100;

        // Write to file
        fseek(fp, 0, SEEK_SET);
        fprintf(fp, "%d\n", random_num);
        fflush(fp);
        printf("Wrote random number %d to file\n", random_num);

        // Release mutex
        pthread_mutex_unlock(&mutex);
        printf("Writer: mutex released!\n");

        // Add delay
        usleep(500000);
    }

    return NULL;
}

int main()
{
    int res;
    pthread_t t1, t2;

    // Open file
    fp = fopen(FILENAME, "a");
    if (fp == NULL)
    {
        printf("Error: could not open file\n");
        exit(EXIT_FAILURE);
    }

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

    // Close file
    fclose(fp);

    return 0;
}
