#include <stdio.h>
#include <stdlib.h>

void inputArray(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        scanf("%d", &(arr[i]));
    }
}

void printArray(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d, ", arr[i]);
    }
}

int sortAscending(int* num1, int* num2) {
    return *num1 - *num2;
}

int sortDescending(int* num1, int* num2) {
    return *num2 - *num1;
}

void sort(int* arr, int size, int (*compare)(int*, int*)) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (compare(&arr[j], &arr[j + 1]) > 0) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int main() {

    int *arr;
    int size;

    printf("Enter array size: ");

    scanf("%d", &size);
    arr = (int*)malloc(sizeof(int) * size);

    if (!arr) {
        printf("Size of the array beyond the permited. Program finishing.\n");
        return -1;
    }

    printf("Enter array elements: ");
    inputArray(arr, size);

    printf("\n\nArray before sorting: ");
    printArray(arr, size);

    printf("\n\nArray in ascending order: ");
    sort(arr, size, sortAscending);
    printArray(arr, size);

    printf("\n\nArray in decreasing order: ");
    sort(arr, size, sortDescending);
    printArray(arr, size);

    printf("\n");

    free(arr);

    return 0;
}
