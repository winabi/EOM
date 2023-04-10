#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define FILENAME "fileSemaphores.txt"

// Global variables
sem_t sem;        // Semaphore to synchronize access to the file
FILE *fp;         // File pointer
char buffer[128]; // Buffer to store data read from file

// Thread functions
void *reader_thread(void *arg)
{
    while (1)
    {
        // Wait for semaphore
        printf("Reader: waiting for semaphore...\n");
        sem_wait(&sem);
        printf("Reader: semaphore acquired!\n");

        // Read from file
        fseek(fp, 0, SEEK_SET);
        fgets(buffer, 128, fp);
        printf("Read from file: %s", buffer);

        // Release semaphore
        sem_post(&sem);
        printf("Reader: semaphore released!\n");

        // Add delay
        usleep(500000);
    }

    return NULL;
}

void *writer_thread(void *arg)
{
    while (1)
    {
        // Wait for semaphore
        printf("Writer: waiting for semaphore...\n");
        sem_wait(&sem);
        printf("Writer: semaphore acquired!\n");

        // Generate random number
        srand(time(NULL));
        int random_num = rand() % 100;

        // Write to file
        fseek(fp, 0, SEEK_SET);
        fprintf(fp, "%d\n", random_num);
        fflush(fp);
        printf("Wrote random number %d to file\n", random_num);

        // Release semaphore
        sem_post(&sem);
        printf("Writer: semaphore released!\n");

        // Add delay
        usleep(500000);
    }

    return NULL;
}

int main()
{
    // Open file
    fp = fopen(FILENAME, "w+");
    if (fp == NULL)
    {
        printf("Error: could not open file\n");
        exit(EXIT_FAILURE);
    }

    // Initialize semaphore
    sem_init(&sem, 0, 1);

    // Create threads
    pthread_t reader_tid, writer_tid;
    pthread_create(&reader_tid, NULL, reader_thread, NULL);
    pthread_create(&writer_tid, NULL, writer_thread, NULL);

    // Wait for threads to finish
    pthread_join(reader_tid, NULL);
    pthread_join(writer_tid, NULL);

    // Cleanup
    sem_destroy(&sem);
    fclose(fp);

    return 0;
}
