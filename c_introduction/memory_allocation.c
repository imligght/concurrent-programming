#include <stdio.h>
#include <stdlib.h>

int main() {


    int* ip = malloc(sizeof(int) * 10);
    free(ip);

    int v[5] = {1, 2, 3, 4, 5};

    return 0;
}
