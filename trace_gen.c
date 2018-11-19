// Generates test data, adjust according to association size
#include <stdio.h>
#define ASSOC 65
#define SET_NUMBER 512
#define OFFSET_WIDTH 3

int main() {
    int x = 1;
    for(int i = 0; i < ASSOC; i++) {
        x <<= OFFSET_WIDTH;
        printf("r %x\n", x);
        x >>= OFFSET_WIDTH;
        x += SET_NUMBER;
    }
}