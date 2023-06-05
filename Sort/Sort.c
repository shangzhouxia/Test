#include <stdio.h>

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

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

void selection_sort(int* arr, int len, int ascending)
{
    int i,j;

    if (!ascending) {
        for (i=0; i<len-1; i++) {
            int min = i;        //假设未排序的部分第一个数最小
            for (j=i+1; j<len; j++) {
                if (arr[j] < arr[min]) {
                    min = j;    //记录最小值
                }
                swap(&arr[min], &arr[i]);
            }
        } 
    } else {
        for (i=0; i<len-1; i++) {
            int max = i;        //假设未排序的部分第一个数最大
            for (j=i+1; j<len; j++) {
                if (arr[j] > arr[max]) {
                    max = j;    //记录最大值
                }
                swap(&arr[max], &arr[i]);
            }
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

/*     printf("\nafter bubbleSort:\n");

    bubbleSort(array, size);
    for (int i=0; i<size; i++) {
        printf("%d ", array[i]);
    } */

    selection_sort(array, size, 0);
    printf("\nafter ascending selection_sort:\n");
    for (int i=0; i<size; i++) {
        printf("%d ", array[i]);
    }

    selection_sort(array, size, 1);
    printf("\nafter deascending selection_sort:\n");
    for (int i=0; i<size; i++) {
        printf("%d ", array[i]);
    }

    printf("\n");

    int a[3][4] = {0,1,2,3, 4,5,6,7, 8,9,10,11};
    int (*p)[4];
    int i,j;
    p = a;

    for (i=0; i<3; i++) {
        for (j=0; j<4; j++) {
            printf("%2d ", *(*(p+i)+j));
        }
        printf("\n");
    }

    char ch;
    char cities[5][10] = {
        "shanghai",
        "beijing",
        "guangzhou",
        "shenzhen",
        "hangzhou"
    };

    char* ptr, **ptrPtr;

    //ptr = cities[0];
    ptrPtr = &ptr;

    for (i=0; i<5; i++) {
        ptr = cities[i];        //  传递行指针
        j=0;
        
        do {
            ch = *(ptr + j);
            printf("%c", ch);
            j++;
        } while (ch != '\0');

        printf("  ");
        j=0;

        do {
            ch = *(*ptrPtr + j);
            printf("%c",ch);
            j++;
        } while (ch != '\0');

        printf("\n");
    }
}