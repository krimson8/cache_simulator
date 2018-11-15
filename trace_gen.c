// Generates test data, adjust according to association size
#include <stdio.h>
#define ASSOC 65
#define SET_NUMBER 512

int main() {
    int x = 0;
    for(int i = 0; i < ASSOC; i++) {
        printf("r %x\n", x);
        x += SET_NUMBER;
    }
}