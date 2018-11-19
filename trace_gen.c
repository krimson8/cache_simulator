// Generates test data, adjust according to association size
#include <stdio.h>
#define ASSOC 65
#define SET_NUMBER 512

int main() {
    int x = 1;
    for(int i = 0; i < ASSOC; i++) {
        x <<= 3;
        printf("r %x\n", x);
        x >>= 3;
        x += SET_NUMBER;
    }
}