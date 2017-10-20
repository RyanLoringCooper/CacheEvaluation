#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <tgmath.h>

#define TRUE 1
#define FALSE 0
#define ROUGHLY_EQUAL_THRESHOLD 5

// will write each index of the passed array to a csv file
void logInts(char *filename, int *data, int arrayLength);

// returns TRUE if are within ROUGHLY_EQUAL_THRESHOLD of each other
int roughlyEqual(int a, int b);

void logStrings(char *filename, int argc, ...);
