#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <sys/types.h>

static int sigusr1_received = 0;

void sigusr1_handler(int sig)
{
    sigusr1_received = 1;
    syslog(LOG_NOTICE, "Received SIGUSR1 signal");
    closelog();
}

void sigint_handler(int sig)
{
    syslog(LOG_NOTICE, "Received SIGINT signal"); //  используется для записи сообщений в журнал системы
    closelog();                                   // закрывает журнал после того, как сообщения были записаны
    exit(0);
}

int main()
{

    pid_t pid;

    openlog("i222Ba7-daemon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0); // открытие журнала
    printf("alarm application starting\n");

    pid = fork();
    switch (pid)
    {
    case -1:
        /* Аварийное завершение */
        syslog(LOG_ERR, "Failed to fork");
        exit(1);
    case 0:
        /* Дочерний процесс */
        setsid(); // отключение от терминала , т.е отвязывает процесс от управляющего терминала, создавая новую сессию и группу процессов

        signal(SIGUSR1, sigusr1_handler); // устанавливаем обработчик сигнала SIGUSR1
        signal(SIGINT, sigint_handler);   // устанавливаем обработчик сигнала SIGINT

        while (1)
        {
            pause();
            if (sigusr1_received)
            {
                syslog(LOG_NOTICE, "Received SIGUSR1 signal");
                sigusr1_received = 0;
            }
        }
    }

    // родительский процесс

    syslog(LOG_NOTICE, "Daemon started with PID %d", pid);
    closelog();
    exit(0);

    return 0;
}
