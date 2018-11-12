#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int tag;
    int index;
    long long content;
}block;

block ***cache_init(int cache_size, int assoc, int block_size) {
    int set_number = cache_size / assoc / block_size;

    block ***cache = malloc(set_number * sizeof(block));

    for(int i = 0; i < set_number; i++) {
        cache[i] = malloc(assoc * sizeof(block));
        for(int j = 0; j < assoc; j++) {
            cache[i][j] = malloc(sizeof(block));
        }
    }
    return cache;
}

int main(int argc, char *argv[]) {
    if(argc != 5) {
        printf("Error: Invalid Argument\n");
        return 0;
    }

    int cache_size = atoi(argv[1]) * 1024;
    int assoc = atoi(argv[2]);
    int block_size = atoi(argv[3]);
    int op_type = atoi(argv[4]);

    block ***cache = cache_init(cache_size, assoc, block_size);
}