#include "util.h"

// will write each index of the passed array to a csv file
void logInts(char *filename, int *data, int arrayLength) {
    FILE *dfile;
    int i;
    if(access(filename, F_OK) != -1) {
        dfile = fopen(filename, "a");
    } else {
        dfile = fopen(filename, "w");
    }
    fprintf(dfile, "\n");
    for(i = 0; i < arrayLength; i++) {
        fprintf(dfile, "%d", data[i]);
        if(i != arrayLength-1) {
            fprintf(dfile, ",");
        }
    }
    fclose(dfile);
}

int roughlyEqual(int a, int b) {
    return abs(a-b) < ROUGHLY_EQUAL_THRESHOLD;
}

void logStrings(char *filename, int argc, ...) {
    va_list argv;
    va_start(argv, argc);
    FILE *dfile;
    int i;
    if(access(filename, F_OK) != -1) {
        dfile = fopen(filename, "a");
    } else {
        dfile = fopen(filename, "w");
    }
    fprintf(dfile, "\n");
    for(i = 0; i < argc; i++) {
        fprintf(dfile, va_arg(argv, char *));
    }
    va_end(argv);
    fclose(dfile);
}

void print(int **arr, int rows, int columns, FILE *loc) {
    int i, j;
    fprintf(loc, "[");
    for(i = 0; i < rows; i++) {
        fprintf(loc, "[");
        for(j = 0; j < columns; j++) {
            fprintf(loc, "%d", arr[i][j]);
            if(j != columns-1) {
                fprintf(loc, ",\t");
            }
        }
        fprintf(loc, "]");
        if(i != rows-1) {
            fprintf(loc, ",\n ");
        } else {
            fprintf(loc, "]\n");
        }
    }
}

int min(int a, int b) {
    if(a < b) {
        return a;
    } else {
        return b;
    }
}
