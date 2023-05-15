#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_SIZE 100

void quick_sort(int arr[], int low, int high)
{
    if (low < high)
    {
        // выбираем опорный элемент
        int pivot = arr[high];

        // разделяем массив на две части
        int i = low - 1;
        for (int j = low; j <= high - 1; j++)
        {
            if (arr[j] <= pivot)
            {
                i++;
                // меняем местами элементы
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        // меняем местами опорный элемент и элемент, стоящий после всех меньших элементов
        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        // рекурсивно сортируем две части массива
        quick_sort(arr, low, i);
        quick_sort(arr, i + 2, high);
    }
}

int main()
{
    int fd;
    int data[MAX_SIZE];
    int count;

    // создаем неименованный канал
    mkfifo("myfifo", 0666);

    // открываем канал для чтения
    fd = open("myfifo", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // читаем данные из канала
    count = read(fd, data, sizeof(data)); // возвращает количество байт
    if (count == -1)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // преобразуем байты в целочисленный массив
    int n = count / sizeof(int);

    // сортируем массив
    quick_sort(data, 0, n - 1);

    // выводим отсортированный массив на экран
    for (int i = 0; i < n; i++)
    {
        printf("%d ", data[i]);
    }
    printf("\n");

    // закрываем канал
    close(fd);

    // удаляем канал
    unlink("myfifo");

    return 0;
}
