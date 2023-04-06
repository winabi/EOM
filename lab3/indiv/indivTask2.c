#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    char *prog = "indivTask1";
    char *args[] = {"./indivTask1", "arg1", NULL};

    pid_t pid;
    if ((pid = fork()) < 0)
    {
        exit(-1);
    }
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

    return 0;
}