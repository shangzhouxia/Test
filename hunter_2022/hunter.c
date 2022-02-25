#include <stdio.h>

//与0异或，得到原始bit值，与1异或，取反
#define bit_swap(data, m , n) \
    (data & (1<<m)) == (data & (1<<n)) ? data : data ^ ((1<<m) | (1<<n))


//一个是他实现的条件必须是一个有序的数列，另外一个是他只能一次性找一个数而不是多个
int binary_search(const int arr[], const int arr_num, const int target)
{
    int left = 0;
    int right = arr_num-1;
    int mid = 0;

    while (left < right) {
        mid = left + (right - left) / 2;

        //若n/2处元素小于我们要查找的元素，则以n/2处为左边界,继续查找右半部分元素
        if (arr[mid] < target) {
            left = mid + 1;
        //若n/2处元素大于我们要查找的元素，则以n/2处为右边界，继续查找左半部分元素
        } else if (arr[mid] > target) {
            right = mid -1;
        } else {
            return mid;
        }
    }

    return -1;
}
/*
4| 1 char
 | 1 char
 | 2 填充

4| 4 int
*/
struct _A {
    char a;
    char b;
    int c;
} A;

/*
4| 1 char
 | 3 填充

4| 4 int

4| 1 char
 | 3 填充
*/
struct _B {
    char a;
    int b;
    char c;
} B;

void main(void)
{
    int data = 9;

    int m=1, n=3;

    printf("%d ->bit_swap(1,3) ->%d.\n", data, bit_swap(data, 1, 3));

    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int i = 0;
    int sz = sizeof(arr) / sizeof(arr[0]);
    int t = 4;

    i = binary_search(arr, sz, t);
    if (i == -1) {
        printf("找不到\n");
    } else {
        printf("找到了, 下标是%d\n", i);
    }

    printf("struct A size:%d\n",sizeof(A));
    printf("struct B size:%d\n",sizeof(B));
}