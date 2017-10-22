#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tgmath.h>
#include "headers/util.h"

#define BUFF_SIZE 256*1024
#define BUFF_INCREMENT 32
#define EPOCHS 1000000

// change this if you are not finding your L1 cache size
#define CACHE_SIZE_TIMEOUT 30
#define LINE_SIZE_TIMEOUT 50

#define CACHE_H_START 8
#define CACHE_H_STOP 20
#define LINE_H_START 0 
#define LINE_H_STOP 8
#define MAX_ASS 32
#define ASS_THRESH 50

char buff[BUFF_SIZE] = "";
char cacheSizeLogFile[] = "cacheSize.log";
char lineSizeLogFile[] = "lineSize.log";
char assLogFile[] = "associativity.log";

int getCacheSize() {
    int h,i,j,k, timeIndex = 0, *aveTimes = (int *) malloc(sizeof(int)*(CACHE_H_STOP-CACHE_H_START));
    int prev_time = 0;
    unsigned long retval = -1;
    for (h=CACHE_H_START; h<CACHE_H_STOP; h++) {
        int test_size = pow(2,h);
        int avg = 0;
        for (i=0; i<10; i++) {
            clock_t start = clock(), diff;
            int arr[test_size];
            for (j=0; j<pow(2,28)/test_size; j++) {
                for (k=0; k<test_size; k+=16) {
                    arr[k] = k;
                }
            }
            diff = clock() - start;
            avg += diff * 1000 / CLOCKS_PER_SEC;
        }
        if (prev_time != 0 && abs(avg/10 - prev_time) > CACHE_SIZE_TIMEOUT) {
            /*printf("Cache size:\t%luB = %gKB\n",
                test_size/2*sizeof(int),
                pow(2,h-11)*sizeof(int)); */
            retval = test_size/2*sizeof(int);
            aveTimes[timeIndex] = avg/10;
            break;
        }
        prev_time = avg/10;
        printf("%lu:\t%d ms\n",
            test_size*sizeof(int), prev_time);
        aveTimes[timeIndex++] = prev_time;
    }
    
    char temp[BUFF_INCREMENT] = ""; 
    sprintf(temp, "%luB", retval);
    logStrings(cacheSizeLogFile, 3, "\nCache size ", temp, " Timings:");
    logInts(cacheSizeLogFile, aveTimes, h-CACHE_H_START);
    printf("Logged cache size timings to %s\n", cacheSizeLogFile);
    free(aveTimes);
    return retval;
}

int getLineSize(int cacheSize) {
    int h,i,j,k, timeIndex = 0, *aveTimes = (int *) malloc(sizeof(int)*(LINE_H_STOP-LINE_H_START));
    int prev_time = 0;
    unsigned long retval = 0;
    for (h=LINE_H_START; h<LINE_H_STOP; h++) {
        int lineSize = pow(2,h);
        int avg = 0;
        for (i=0; i<10; i++) {
            int arr[cacheSize*2];
	    clock_t start = clock(), diff;
	    for (j=0; j<1000; j++) {
                for (k=0; k<cacheSize*2/lineSize; k++) {
                    arr[k*lineSize] = k;
                }
            }
            diff = clock() - start;
            avg += diff * 1000 / CLOCKS_PER_SEC;
        }
        if (prev_time != 0 && abs(avg*lineSize/10 - prev_time) > LINE_SIZE_TIMEOUT) {
            retval = lineSize*sizeof(int);
            aveTimes[timeIndex] = avg*lineSize/10;
            break;
        }
        printf("%lu:\t%d\n",lineSize*sizeof(int),avg*lineSize/10);
        prev_time = avg*lineSize/10;
        aveTimes[timeIndex++] = prev_time;
    }
    char temp[BUFF_INCREMENT] = "";
    sprintf(temp, "%luB", retval);
    logStrings(lineSizeLogFile, 3, "\nLine size ", temp, " Timings:");
    logInts(lineSizeLogFile, aveTimes, h-LINE_H_START);
    printf("Logged line size timings to %s\n", lineSizeLogFile);
    free(aveTimes);
    return retval;
}

int getAssociativity(int cacheSize) {
    int aveTimeSize = (int)log2(MAX_ASS);
    int h,i,j,k, timeIndex = 0, *aveTimes = (int *) malloc(sizeof(int)*aveTimeSize);
    int prev_time = 0, retval = 1;
    for (h=MAX_ASS; h>0; h/=2) {
        int avg = 0;
        for (i=0; i<10; i++) {
            int arr[cacheSize*2];
            clock_t start = clock(), diff;
            for (j=0; j<40000000; j++) {
                for (k=0; k<h+1; k++) {
                    arr[k*cacheSize/h] = k;
                }
            }
            diff = clock() - start;
            avg += diff * 1000 / CLOCKS_PER_SEC;
        }
        prev_time = avg/10/(h+1);
        aveTimes[timeIndex++] = prev_time;
        printf("%d:\t%d\n",h,prev_time);
    }
    for(i = 2; i < aveTimeSize; i++) {
       if(aveTimes[i-1]-aveTimes[i] < ASS_THRESH && aveTimes[i-2]-aveTimes[i-1] > ASS_THRESH) {
            retval = MAX_ASS/pow(2,i-1);
        }
    }
    char temp[BUFF_INCREMENT] = "";
    sprintf(temp, "%d", retval);
    logStrings(assLogFile, 3, "\nAssociativity ", temp, " Timings:");
    logInts(assLogFile, aveTimes, aveTimeSize);
    printf("Logged associativity to %s\n", assLogFile);
    free(aveTimes);
    return retval;
}

int main(int argc, char *argv[]) {
    int cacheSize, lineSize, associativity;
    printf("Finding cache size...\n");
    cacheSize = getCacheSize();
    printf("Cache size: %d\n\nFinding line size...\n", cacheSize);
    lineSize = getLineSize(cacheSize);
    printf("Line size: %d\n\nFinding associativity...\n", lineSize);
    associativity = getAssociativity(cacheSize);
    printf("Associativity: %d\n", associativity);
    return 0;
}
