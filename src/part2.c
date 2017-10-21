#include <stdio.h>
#include <stdlib.h>
#include "headers/util.h"

#define SIDE_LENGTH 8

int ***getArrays(int sideLength) {
    int ***arr = (int ***) malloc(sizeof(int **)*2);
    int i, j;
    arr[0] = (int **) malloc(sizeof(int *)*sideLength);
    arr[1] = (int **) malloc(sizeof(int *)*sideLength);
    for(i = 0; i < sideLength; i++) {
        arr[0][i] = (int *) malloc(sizeof(int)*sideLength);
        arr[1][i] = (int *) malloc(sizeof(int)*sideLength);
        for(j = 0; j < sideLength; j++) {
            arr[0][i][j] = i;
            arr[1][i][j] = j;
        }
    }
    return arr;
}

// will only multiply square matricies with the same shape
int **multiplySquareMatricies(int **a, int **b, int sideLength) {
    int **c = (int **) malloc(sizeof(int *)*sideLength);
    int i;
    for(i = 0; i < sideLength; i++) {
        c[i] = (int *) malloc(sizeof(int)*sideLength);
    }
    // TODO multiplication goes here
    return c;
}

int main(int argc, char *argv[]) {
    int ***arrs = getArrays(SIDE_LENGTH);
    int **result, **hori = arrs[0], **vert = arrs[1];
    int i;
    printf("hori = \n");
    print(hori, SIDE_LENGTH, SIDE_LENGTH, stdout);
    printf("vert = \n");
    print(vert, SIDE_LENGTH, SIDE_LENGTH, stdout);

    result = multiplySquareMatricies(hori, vert, SIDE_LENGTH);
    printf("hori * vert = \n");
    print(result, SIDE_LENGTH, SIDE_LENGTH, stdout);

    for(i = 0; i < SIDE_LENGTH; i++) {
        free(hori[i]);
        free(vert[i]);
        free(result[i]);
    }
    free(hori);
    free(vert);
    free(result);
    free(arrs);
    return 0;
}
