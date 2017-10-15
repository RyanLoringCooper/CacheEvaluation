#include <stdio.h>
#include "../src/headers/util.h"

void testLogInts() {
    int data[7] = { 1, 2, 3, 4, 5, 6, 7 };
    logInts(data, 7, "testFile");
}

int main(int argc, char *argv[]) {
    testLogInts();
    return 0;
}


