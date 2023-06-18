#ifndef FREQ_BYTE_MAP_H
#define FREQ_BYTE_MAP_H

struct freq_interval_t {
    int min, max;
};
struct freq_byte_pair_t {
    struct freq_interval_t interval;
    unsigned char byte;
};

struct freq_byte_pair_t* freq_byte_map_create(int offset, int start_freq, int* max_calculated_freq);
int find_idx_by_byte(struct freq_byte_pair_t* map, unsigned char byte);
int find_idx_by_freq(struct freq_byte_pair_t* map, int freq);

#endif // FREQ_BYTE_MAP_H