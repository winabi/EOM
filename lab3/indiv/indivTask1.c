#include <stdio.h>

void quicksort(int arr[], int low, int high)
{
    int i = low, j = high;
    int pivot = arr[(low + high) / 2];

    while (i <= j)
    {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j)
        {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
    }

    if (low < j)
        quicksort(arr, low, j);
    if (i < high)
        quicksort(arr, i, high);
}

int main()
{

    int size;
    printf("Enter the size of the array: ");
    scanf("%d", &size);

    int arr[size];
    for (int i = 0; i < size; i++)
    {
        printf("Enter a %d number: ", i + 1);
        scanf("%d", &arr[i]);
    }

    quicksort(arr, 0, size - 1);

    // Вывод массива на экран
    printf("Output the array to the console\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}