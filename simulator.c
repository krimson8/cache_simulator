#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
    int valid;
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
            cache[i][j].valid = 0;
        }
    }

    // printf("Created a cache with %d set, each with %d blocks.\n", set_number, assoc);
    return cache;
}

int exist_in_cache(block **cache, int assoc, int input_set, unsigned long long input_dec) {
    // check if exists in the cache set
    for(int i = 0; i < assoc; i++) {
        if(cache[input_set][i].tag == input_dec && cache[input_set][i].valid == 1) {
            cache[input_set][i].time_stamp = 0; 
            return 1;
        }
    }
    return 0;
}

int write_to_cache(block **cache, int assoc, int input_set, unsigned long long input_dec) {
    for(int i = 0; i < assoc; i++) {
        if(cache[input_set][i].valid == 0) {
            cache[input_set][i].valid = 1;
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

void random_replacement(block **cache, int assoc, int input_set, unsigned long long input_dec) {
    int r = rand() % assoc;
    cache[input_set][r].tag = input_dec;
}

void increment_timestamp(block **cache, int assoc, int input_set) {
    for(int i = 0; i < assoc; i++) {
        cache[input_set][i].time_stamp++;
    }
}

int count_offset(int block_size) {
    // block_size /= 8; // turn from bits to bytes
    int count = 0;
    while(block_size > 1) {
        block_size /= 2;
        count++;
    }
    return count;
}

int main(int argc, char *argv[]) {
    if(argc != 5) {
        printf("Error: Invalid Argument\n");
        return 0;
    }

    int cache_size = atoi(argv[1]) * 1024;
    int assoc = atoi(argv[2]);
    int block_size = atoi(argv[3]);
    char op_type[2];
    strcpy(op_type, argv[4]);

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
    // count offset
    int offset_width = count_offset(block_size);
    // printf("Offset = %d bits\n", offset_width);

    block **cache = cache_init(cache_size, assoc, block_size);
    srand(time(NULL));
    while(scanf("%s %s", rw, input) != EOF) {
        unsigned long long input_dec = strtoull(input, &pEND, 16);

        input_dec >>= offset_width; // dispose offset;
        int input_set = input_dec & (set_number - 1);
        input_dec = (input_dec | (set_number - 1)) ^ (set_number - 1);

        // printf("%s %llu -- input_set = %d\n", rw, input_dec, input_set);

        total_access++;

        if(!strcmp(rw, "r")) {
            read_access++;
            int hit = exist_in_cache(cache, assoc, input_set, input_dec);
            if(!hit) {
                int write_success = write_to_cache(cache, assoc, input_set, input_dec);
                
                if(!write_success) {
                    if(!strcmp(op_type, "l")) {
                        lru_replacement(cache, assoc, input_set, input_dec);
                    }
                    else if(!strcmp(op_type, "r")) {
                        random_replacement(cache, assoc, input_set, input_dec);
                    }
                    else {
                        printf("Error: Invalid Argument (l or r)\n");
                        return 0;
                    } 
                }
                total_miss++;
                read_miss++;
            }
            increment_timestamp(cache, assoc, input_set);
        }
        else if(!strcmp(rw, "w")) {
            write_access++;
            int hit = exist_in_cache(cache, assoc, input_set, input_dec);
            if(!hit) {
                int write_success = write_to_cache(cache, assoc, input_set, input_dec);
                
                if(!write_success) {
                    if(!strcmp(op_type, "l")) {
                        lru_replacement(cache, assoc, input_set, input_dec);
                    }
                    else if(!strcmp(op_type, "r")) {
                        random_replacement(cache, assoc, input_set, input_dec);
                    }
                    else {
                        printf("Error: Invalid Argument (l or r)\n");
                        return 0;
                    } 
                }
                total_miss++;
                write_miss++;
            }
            increment_timestamp(cache, assoc, input_set);
        }
    }

    double miss_rate = ((double)total_miss/total_access) * 100;
    double read_miss_rate = ((double)read_miss/read_access) * 100;
    double write_miss_rate = ((double)write_miss/write_access) * 100;

    printf("%d %.6lf%% %d %.6lf%% %d %.6lf%%\n", total_miss, miss_rate, read_miss, read_miss_rate, write_miss, write_miss_rate);
    return 0;
}
