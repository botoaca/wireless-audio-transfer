#include "freq_byte_map.h"
#include <stdlib.h>

struct freq_byte_pair_t* freq_byte_map_create(int offset, int start_freq, int deadzone, int* max_calculated_freq) {
    struct freq_byte_pair_t* map = malloc(sizeof(struct freq_byte_pair_t) * 256);
    map[0].interval.min = start_freq;
    map[0].interval.max = start_freq + offset;
    map[0].byte = 0;
    for (int i = 1; i < 256; i++) {
        map[i].interval.min =  map[i - 1].interval.max + deadzone;
        map[i].interval.max =  map[i - 1].interval.max + offset;
        map[i].byte = i;
    }
    *max_calculated_freq = map[255].interval.max;
    return map;
}

int find_idx_by_byte(struct freq_byte_pair_t* map, char byte) {
    for (int i = 0; i < 256; i++) {
        if (map[i].byte == byte) return i;
    }
    return -1;
}