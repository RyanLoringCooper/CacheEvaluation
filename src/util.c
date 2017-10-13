#include "util.h"

// will write each index of the passed array to a csv file
void logInts(int *data, int arrayLength, char *filename) {
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
