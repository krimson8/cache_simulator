#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int occupied;
    unsigned long long tag;
    unsigned long long time_stamp;
}block;

block **cache_init(int cache_size, int assoc, int block_size) {
    int set_number = cache_size / assoc / block_size;

    block **cache = (block **)malloc(set_number * sizeof(block*));

    for(int i = 0; i < set_number; i++) {
        cache[i] = (block *)malloc(assoc * sizeof(block));
    }

    for(int i = 0; i < set_number; i++) {
        for(int j = 0; j < assoc; j++) {
            cache[i][j].time_stamp = 0;
            cache[i][j].tag = 0;
            cache[i][j].occupied = 0;
        }
    }

    printf("Created a cache with %d set, each with %d blocks.\n", set_number, assoc);
    // for(int i = 0; i < set_number; i++) {
    //     for(int j = 0; j < assoc; j++) {
    //         printf("%lld ", cache[i][j].tag);
    //     }
    //     printf("\n");
    // }
    return cache;
}

int exist_in_cache(block **cache, int assoc, int input_set, unsigned long long input_dec) {
    for(int i = 0; i < assoc; i++) {
        if(cache[input_set][i].tag == input_dec) {
            cache[input_set][i].time_stamp = 0; 
            return 1;
        }
    }
    return 0;
}

int write_to_cache(block **cache, int assoc, int input_set, unsigned long long input_dec) {
    for(int i = 0; i < assoc; i++) {
        if(cache[input_set][i].occupied == 0) {
            cache[input_set][i].occupied = 1;
            cache[input_set][i].tag = input_dec;
            cache[input_set][i].time_stamp = 0; 
            return 1;
        }
    }
    return 0;
}

void lru_replacement(block **cache, int assoc, int input_set, unsigned long long input_dec) {
    unsigned long long max = 0;
    int lru_entry = 0;
    for(int i = 0; i < assoc; i++) {
        if(cache[input_set][i].time_stamp > max) {
            max = cache[input_set][i].time_stamp;
            lru_entry = i;
        }
    }
    cache[input_set][lru_entry].tag = input_dec;
    cache[input_set][lru_entry].time_stamp = 0; 
}

void increment_timestamp(block **cache, int assoc, int input_set) {
    for(int i = 0; i < assoc; i++) {
        cache[input_set][i].time_stamp++;
    }
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

    char rw[2];
    char input[16];
    char *pEND;

    int total_access = 0;
    int total_miss = 0;
    int read_access = 0;
    int read_miss = 0;
    int write_access = 0;
    int write_miss = 0;

    int set_number = cache_size / assoc / block_size;

    block **cache = cache_init(cache_size, assoc, block_size);
    while(scanf("%s %s", rw, input) != EOF) {
        unsigned long long input_dec = strtoll(input, &pEND, 16);

        int input_set = input_dec % set_number;
        // printf("%s %lld -- input_set = %d\n", rw, input_dec, input_set);
        int offset_bit = count_offset();

        total_access++;

        if(!strcmp(rw, "r")) {
            read_access++;
            int hit = exist_in_cache(cache, assoc, input_set, input_dec);
            if(!hit) {
                // printf("Hit missed -");
                int write_success = write_to_cache(cache, assoc, input_set, input_dec);
                
                if(!write_success) {
                    // printf("Write missed - Do LRU");
                    lru_replacement(cache, assoc, input_set, input_dec);
                }
                // printf("\n");
                total_miss++;
                read_miss++;
            }
            else {
                // printf("Hit success\n");
            }
            
            increment_timestamp(cache, assoc, input_set);
        }
        else if(!strcmp(rw, "w")) {
            write_access++;
            int hit = exist_in_cache(cache, assoc, input_set, input_dec);
            if(!hit) {
                // printf("Hit missed -");
                int write_success = write_to_cache(cache, assoc, input_set, input_dec);
                
                if(!write_success) {
                    // printf("Write missed - Do LRU");
                    lru_replacement(cache, assoc, input_set, input_dec);
                }
                // printf("\n");
                total_miss++;
                write_miss++;
            }
            else {
                // printf("Hit success\n");
            }

            increment_timestamp(cache, assoc, input_set);
        }
    }

    printf("\nTotal miss = %d\n", total_miss);
    printf("Total access = %d\n", total_access);
    double miss_rate = ((double)total_miss/total_access) * 100;
    printf("Miss rate = %.2lf%%\n", miss_rate);

    printf("\nRead miss = %d\n", read_miss);
    printf("Read access = %d\n", read_access);
    double read_miss_rate = ((double)read_miss/read_access) * 100;
    printf("Read miss rate = %.2lf%%\n", read_miss_rate);

    printf("\nWrite miss = %d\n", write_miss);
    printf("Write access = %d\n", write_access);
    double write_miss_rate = ((double)write_miss/write_access) * 100;
    printf("Write miss rate = %.2lf%%\n", write_miss_rate);

    return 0;
}