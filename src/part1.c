#include <stdio.h>
#include <time.h>
#include <tgmath.h>
#include "headers/util.h"

#define BUFF_SIZE 256*1024
#define BUFF_INCREMENT 32
#define EPOCHS 1000000

char buff[BUFF_SIZE] = "";

clock_t getMissTime() {
    int i, j;
    clock_t start, end;
    char temp;
    start = clock();
    for(i = 0; i < EPOCHS; i++) {
        for(j = 0; j < BUFF_SIZE; j += BUFF_INCREMENT) {
            temp = buff[j];
        }
    }
    end = clock();
    return (end-start)/EPOCHS;
}

int getCacheSize(clock_t missTime) {
    int i, j; 
    char temp;
    clock_t start, end, times[(int)log2(BUFF_SIZE)];
    int limitReached = FALSE, cacheSize = 0, timeIndex = 0;
    // make sure the first access is a hit, so that in the first 
    // iteration of the loop, end-start is not roughly equal to the time
    // a miss would take.
    temp = buff[0];
    for(i = 0; i < BUFF_SIZE && !limitReached; i *= 2) {
        for(j = 0; j < i; j += BUFF_INCREMENT) {
            temp = buff[j];
        }
        start = clock();
        temp = buff[0];
        end = clock();
        times[timeIndex++] = end-start;
        if(roughlyEqual(end-start, missTime)) {
            cacheSize = i;
            limitReached = TRUE;
        }
    }
    logInts((int *) &times[0], (int)log2(cacheSize), "getCacheSizeTimes.log");
    return cacheSize;
}

int getLineSize() {
    // TODO
    return 0;
}

int getAssociativity() {
    // TODO
    return 0;
}

int main(int argc, char *argv[]) {
    int cacheSize, lineSize, associativity;
    clock_t missTime = getMissTime();
    cacheSize = getCacheSize(missTime);
    printf("Cache size: %d\n", cacheSize);
    lineSize = getLineSize();
    printf("Line size: %d\n", lineSize);
    associativity = getAssociativity();
    printf("Associativity: %d\n", associativity);
}
