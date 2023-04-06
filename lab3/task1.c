#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *prog = "task2";
    char *args[] = {"./task2", "arg1", NULL};

    int async_flag = 0;

    // Проверяем правильность использования приложения
    if (argc != 3 || (strcmp(argv[1], "-w") != 0 && strcmp(argv[1], "-f") != 0))
    {
        printf("Usage: %s [-w|-f] filename\n", argv[0]);
        return -1;
    }

    // Задаем имя файла для запуска
    prog = argv[2];

    pid_t pid;
    if (strcmp(argv[1], "-w") == 0)
    {
        if ((pid = fork()) < 0)
            exit(-1);
        if (pid == 0)
        {
            // потомок загружает вместо себя другую

            if (execvp(prog, args) == -1)
            {
                printf("Error starting the child\n");
                exit(-1);
            }
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }
    else if (strcmp(argv[1], "-f") == 0)
    {
        pid_t pid_child;
        if ((pid = fork()) < 0)
            exit(-1);
        // (1) первый fork()
        if (pid == 0)
        { // потомок 1
            if ((pid_child = fork()) < 0)
                exit(-1); // (3) второй fork()
            if (pid_child == 0)
            {
                // (5) потомок 2
                // запуск по exec() асинхронно
                if (execvp(prog, args + 1) == -1)
                {
                    printf("Error starting the child\n");
                    exit(-1);
                }
            }
            else
            {
                // (4) потомок 1, он же
                printf("Trying to start a child with a pid=%d\n", pid_child);
                exit(0);
                // выход из потомка 1
            }
        }
        else
        { // предок 1
            waitpid(pid, NULL, 0);
            // (2) ожидание завершения потомка
        }
    }

    return 0;
}
