#include <stdio.h>

void bubbleSort(int* arr, const int arr_size)
{
    int temp;
    int swapFlag = 0;
    int i,j = 0;

    for (i=0; i<arr_size-1; i++) {
        swapFlag = 0;
        for (j=0; j<arr_size-1-i; j++) {
            if (arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;

                swapFlag = 1;
            }
        }
        if (!swapFlag) {
            break;
        }
    }
}

void main(void)
{
    int array[7] = {56, 34, 78, 12, 67, 100, 9};

    int size = sizeof(array)/sizeof(array[0]);

    printf("Original array:\n");
    for (int i=0; i<size; i++) {
        printf("%d ", array[i]);
    }

    printf("\nafter bubbleSort:\n");

    bubbleSort(array, size);
    for (int i=0; i<size; i++) {
        printf("%d ", array[i]);
    }
}