#ifndef FREQ_BYTE_MAP_H
#define FREQ_BYTE_MAP_H

struct freq_interval_t {
    int min, max;
};
struct freq_byte_pair_t {
    struct freq_interval_t interval;
    char byte;
};

struct freq_byte_pair_t* freq_byte_map_create(int offset, int start_freq, int deadzone, int* max_calculated_freq);
int find_idx_by_byte(struct freq_byte_pair_t* map, char byte);

#endif // FREQ_BYTE_MAP_H