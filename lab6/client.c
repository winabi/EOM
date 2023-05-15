#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_SIZE 100

int main()
{
    int fd;
    int data[MAX_SIZE];
    int n;

    // создаем неименованный канал
    mkfifo("myfifo", 0666); // 0666 - маска доступа, разрешающее процессу с аналогичными реквизитами пользователя чтение и запись

    // открываем канал для записи
    fd = open("myfifo", O_WRONLY);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // запрашиваем у пользователя ввод целочисленного вектора
    printf("Введите целочисленный вектор: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &data[i]);
    }

    // записываем массив в канал
    int count = n * sizeof(int);
    write(fd, data, count);

    // закрываем канал
    close(fd);

    // удаляем канал
    unlink("myfifo");

    return 0;
}
