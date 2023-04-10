#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_CHAIRS 5    // количество стульев
#define HAIR_CUT_TIME 3 // время стрижки в секундах

int num_customers = 0;                              // количество клиентов в очереди
int next_seat = 0;                                  // следующий свободный стул в очереди
int next_customer = 0;                              // следующий клиент, который будет обслужен
int sleeping = 1;                                   // флаг, указывающий на спящего парикмахера
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // мьютекс для синхронизации доступа к общим данным
pthread_cond_t barber = PTHREAD_COND_INITIALIZER;   // условная переменная для ожидания парикмахера
pthread_cond_t customer = PTHREAD_COND_INITIALIZER; // условная переменная для ожидания клиента

void *barber_function(void *arg)
{
    while (1)
    {
        // захватываем мьютекс
        pthread_mutex_lock(&mutex);
        if (num_customers == 0)
        {
            // если нет клиентов, парикмахер засыпает
            printf("Barber is sleeping...\n");
            sleeping = 1;
            // ожидаем условие, что есть клиент
            pthread_cond_wait(&barber, &mutex);
        }
        // парикмахер просыпается и готов к стрижке
        sleeping = 0;
        // выбираем следующего клиента в очереди
        next_customer = (next_customer + 1) % NUM_CHAIRS;
        num_customers--;
        printf("Barber is cutting hair...\n");
        // освобождаем стул клиента
        pthread_cond_signal(&customer);
        // освобождаем мьютекс
        pthread_mutex_unlock(&mutex);
        // проводим стрижку
        sleep(HAIR_CUT_TIME);
    }
}

void *customer_function(void *arg)
{
    int id = *(int *)arg;
    // ждем случайное время до прихода в парикмахерскую
    sleep(rand() % 10 + 1);
    // захватываем мьютекс
    pthread_mutex_lock(&mutex);
    if (num_customers == NUM_CHAIRS)
    {
        // если все стулья заняты, уходим
        printf("Customer %d is leaving...\n", id);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    // садимся в свободный стул
    num_customers++;
    printf("Customer %d is sitting in chair %d...\n", id, next_seat);
    // выбираем следующий свободный стул в очереди
    next_seat = (next_seat + 1) % NUM_CHAIRS;
    if (sleeping)
    {
        // если парикмахер спит, будим его
        printf("Customer %d is waking the barber...\n", id);
        pthread_cond_signal(&barber);
    }
    // ожидаем, пока парикмахер освободит стул
    pthread_cond_wait(&customer, &mutex);
    // теперь парикмахер обслуживает клиента
    printf("Customer %d is getting a haircut...\n", id);
    // освобождаем мьютекс
    pthread_mutex_unlock(&mutex);
    // проводим стрижку
    sleep(HAIR_CUT_TIME);
    // клиент уходит после стрижки
    printf("Customer %d is leaving after haircut.\n", id);
    return NULL;
}

int main()
{
    srand(time(NULL)); // инициализируем генератор случайных чисел
    pthread_t barber_thread, customer_threads[NUM_CHAIRS];
    int customer_ids[NUM_CHAIRS];
    // создаем поток парикмахера
    pthread_create(&barber_thread, NULL, barber_function, NULL);
    // создаем потоки клиентов
    for (int i = 0; i < NUM_CHAIRS; i++)
    {
        customer_ids[i] = i;
        pthread_create(&customer_threads[i], NULL, customer_function, &customer_ids[i]);
        // ждем случайное время до прихода следующего клиента
        sleep(rand() % 5 + 1);
    }
    // ждем завершения всех потоков клиентов
    for (int i = 0; i < NUM_CHAIRS; i++)
    {
        pthread_join(customer_threads[i], NULL);
    }
    // завершаем поток парикмахера
    pthread_cancel(barber_thread);
    pthread_join(barber_thread, NULL);
    return 0;
}
