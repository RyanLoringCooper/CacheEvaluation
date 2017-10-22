#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "headers/util.h"

#define SIDE_LENGTH 1024
#define MAX_BLOCK_SIZE 32
#define true 1
#define false 0

int **getMatrix(int sideLength) {
    int **matrix = (int **) malloc(sizeof(int *)*sideLength);
    int i, j;
    for(i = 0; i < sideLength; i++) {
        matrix[i] = (int *) malloc(sizeof(int)*sideLength);
    }
    for(i = 0; i < sideLength; i++) {
        for(j = 0; j < sideLength; j++) {
            matrix[i][j] = 0;
        }
    }
    return matrix;
}

int ***getArrays(int sideLength) {
    int ***arr = (int ***) malloc(sizeof(int **)*2);
    int i, j;
    arr[0] = getMatrix(sideLength);
    arr[1] = getMatrix(sideLength);
    for(i = 0; i < sideLength; i++) {
        for(j = 0; j < sideLength; j++) {
            arr[0][i][j] = i;
            arr[1][i][j] = j;
        }
    }
    return arr;
}

int matricesEqual(int **a, int **b, int sideLength){
    int i, j;
    for(i = 0; i < sideLength; i++){
        for(j = 0; j < sideLength; j++){
            if (a[i][j] != b[i][j])
                return false;
        }
    }
    return true;
}

int **badMatrixMultiply(int **a, int **b, int sideLength) {
    int i, j, k, sum;
    int **c = getMatrix(sideLength);
    for(i = 0; i < sideLength; i++) {
        for(j = 0; j < sideLength; j++){
            sum = 0;
            for(k = 0; k < sideLength; k++){
                sum = sum + a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }
    return c;
}

// Pseudocode from http://www.netlib.org/utk/papers/autoblock/node2.html
int **blockingMatrixMultiply(int **a, int **b, int sideLength, int blockSize) {
    int **c = getMatrix(sideLength);
    int i, j, k, ii, jj, kk;
    for (ii = 0; ii < sideLength; ii += blockSize) {
        for(jj = 0; jj < sideLength; jj += blockSize) {
            for (kk = 0; kk < sideLength; kk += blockSize){
                for (i = ii; i < min(sideLength, ii + blockSize); i++){
                    for(j = jj; j < min(sideLength, jj + blockSize); j++){
                        for(k = kk; k < min(sideLength, kk + blockSize); k++){
                            c[i][j] = c[i][j] + a[i][k] * b[k][j];
                        }
                    }
                }
            }
        }
    }
    
    return c;
}

// will only multiply square matricies with the same shape
void multiplySquareMatricies(int **a, int **b, int sideLength) {
    clock_t start, diff;
    int avg = 0, i, blockSize, **c, **d;
    int iterations = 10;
    
    for(i = 0; i < iterations; i++){
        start = clock();
        c = badMatrixMultiply(a, b, sideLength);
        diff = clock() - start;
        avg += diff * 1000 / CLOCKS_PER_SEC;
        free(c);
    }
    printf("Non Blocking Avg Time: %d\n", avg/iterations);
    
    
    for(blockSize = 8; blockSize < MAX_BLOCK_SIZE; blockSize++){
        avg = 0;
        for(i = 0; i < iterations; i++){
            start = clock();
            d = blockingMatrixMultiply(a, b, sideLength, blockSize);
            diff = clock() - start;
            avg += diff * 1000 / CLOCKS_PER_SEC;
            free(d);
        }
        printf("Block Size %d Avg Time: %d\n", blockSize, avg/iterations);
    }
}


int main(int argc, char *argv[]) {
    int ***arrs = getArrays(SIDE_LENGTH);
    int **result, **hori = arrs[0], **vert = arrs[1];
    int i;
    
    multiplySquareMatricies(hori, vert, SIDE_LENGTH);
    //print(result, SIDE_LENGTH, SIDE_LENGTH, stdout);

    for(i = 0; i < SIDE_LENGTH; i++) {
        free(hori[i]);
        free(vert[i]);
        //free(result[i]);
    }
    free(hori);
    free(vert);
    //free(result);
    free(arrs);
    return 0;
}
