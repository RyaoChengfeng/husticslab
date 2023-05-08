#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>

typedef struct {
    int valid;
    unsigned int tag;
    int times;
} Cache;
char *filepath = NULL;
int s, E, b, s_num;
int hit = 0, miss = 0, eviction = 0;
Cache **cache;

void LoadCache(unsigned address) {
    unsigned s_address = (address >> b) & ((0xffffffff) >> (32 - s));
    unsigned t_address = address >> (s + b);
    // 是否命中cache
    for (int i = 0; i < E; i++) {
        if (cache[s_address][i].tag == t_address) {
            cache[s_address][i].times = 0;
            hit++;
            return;
        }
    }

    for (int i = 0; i < E; i++) {
        if (cache[s_address][i].valid == 0) {
            cache[s_address][i].tag = t_address;
            cache[s_address][i].valid = 1;
            cache[s_address][i].times = 0;
            miss++;
            return;
        }
    }
    // lru 操作
    int max_stamp = 0;
    int max_i = 0;
    for (int i = 0; i < E; i++) {
        if (cache[s_address][i].times > max_stamp) {
            max_stamp = cache[s_address][i].times;
            max_i = i;
        }
    }
    eviction++;
    miss++;
    cache[s_address][max_i].tag = t_address;
    cache[s_address][max_i].times = 0;
}

int main(int argc, char *argv[]) {
    int opt = 0;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (opt) {
            case 's':
                s = atoi(optarg);
                s_num = 1 << s;
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                filepath = optarg;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }
    // malloc cache[s_num][E]
    cache = (Cache **) malloc(sizeof(Cache *) * s_num);
    for (int i = 0; i < s_num; i++) {
        cache[i] = (Cache *) malloc(sizeof(Cache) * E);
    }
    for (int i = 0; i < s_num; i++) {
        for (int j = 0; j < E; j++) {
            cache[i][j].valid = 0;
            cache[i][j].tag = 0xffffffff;
            cache[i][j].times = 0;
        }
    }
    // read file
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        exit(-1);
    }
    char operation;
    unsigned int address;
    int size;
    while (fscanf(file, " %c %x,%d", &operation, &address, &size) > 0) {
        int op = 0;
        switch (operation) {
            case 'I':
                op = 0;
                break;
            case 'L':
                op = 1;
                break;
            case 'M':
                op = 2;
                break;
            case 'S':
                op = 1;
                break;
            default:
                break;
        }
        if (op == 1) {
            LoadCache(address);
        } else if (op == 2) {
            LoadCache(address);
            hit++;
        }

        for (int i = 0; i < s_num; i++) {
            for (int j = 0; j < E; j++) {
                if (cache[i][j].valid == 1)
                    cache[i][j].times++;
            }
        }
    }

    // free cache
    for (int i = 0; i < s_num; i++) {
        free(cache[i]);
    }
    free(cache);

    fclose(file);
    printSummary(hit, miss, eviction);
    return 0;
}