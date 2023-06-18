#include "freq_byte_map.h"
#include <stdlib.h>

struct freq_byte_pair_t* freq_byte_map_create(int offset, int start_freq, int* max_calculated_freq) {
    struct freq_byte_pair_t* map = malloc(sizeof(struct freq_byte_pair_t) * 256);
    map[0].interval.min = start_freq;
    map[0].interval.max = start_freq + offset;
    map[0].byte = 0;
    for (int i = 1; i < 256; i++) {
        map[i].interval.min =  map[i - 1].interval.max + 1;
        map[i].interval.max =  map[  i  ].interval.min + offset;
        map[i].byte = i;
    }
    *max_calculated_freq = map[255].interval.max;
    return map;
}

int find_idx_by_byte(struct freq_byte_pair_t* map, unsigned char byte) {
    for (int i = 0; i < 256; i++) {
        if (map[i].byte == byte) return i;
    }
    if (byte == 0) return 0;
    return -1;
}

int find_idx_by_freq(struct freq_byte_pair_t* map, int freq) {
    for (int i = 0; i < 256; i++) {
        if (map[i].interval.min <= freq && freq <= map[i].interval.max) return i;
    }
    if (freq == 0) return 0;
    return -1;
}