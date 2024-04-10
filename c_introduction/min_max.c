#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int min;
    int max;
} MinMax;

// Takes an array and its size, return its minimum and maximum values
// as a pointer to a structure
MinMax* getMinMax(int* array, const int SIZE) {

    MinMax* array_min_max = (MinMax*) malloc(sizeof(MinMax));

    int min = array[0];
    int max = array[0];

    for (int i = 0; i < SIZE; i++) {
        if (array[i] < min) {
            min = array[i];
        }
        if (array[i] > max) {
            max = array[i];
        }
    }

    array_min_max->min = min;
    array_min_max->max = max;

    return array_min_max;
}

int main() {

    int arr[7] = {8, 9, 1, 2, 3, 6, 5};

    MinMax* result = getMinMax(arr, 7);

    printf("Minimum: %d\nMaximum: %d\n", result->min, result->max);

    free(result);

    return 0;
}
