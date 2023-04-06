#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    printf("Command line argument values:\n");
    for (int i = 0; i < argc; i++)
    {
        printf("%s", argv[i]);
        printf("\n");
    }

    printf("PID %d\n", getpid());
    printf("SID %d\n", getsid(0));
    printf("PGID %d\n", getpgid(0));
    printf("PPID %d\n", getppid());

    struct passwd *pwd;
    pwd = getpwuid(getuid());
    printf("User %s\n", pwd->pw_name);

    struct group *grp;
    grp = getgrgid(getgid());
    printf("User's group %s\n", grp->gr_name);

    return 0;
}