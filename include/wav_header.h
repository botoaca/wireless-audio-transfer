#ifndef WAV_HEADER_H
#define WAV_HEADER_H

struct wav_header_t {
    char RIFF[4];
    int size;
    char WAVE[4];
    char fmt[4];
    int fmt_size;
    short fmt_type;
    short channels;
    int sample_rate;
    int byte_rate;
    short block_align;
    short bit_depth;
    char data[4];
    int data_size;
};

#endif // WAV_HEADER_H