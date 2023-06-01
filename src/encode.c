#include "encode.h"

void encode(struct args_t args)
{
    int max_calculated_freq = 0;
    struct freq_byte_pair_t* map = freq_byte_map_create(10, /* offset */
                                                        10, /* start_freq */
                                                        5,  /* deadzone */
                                                        &max_calculated_freq);
    
    FILE* input_file = fopen(args.file, "rb");              // open file
    void* input_file_data = NULL;

    if (!input_file) { printf("Failed to open input file\n"); return; }
    fseek(input_file, 0, SEEK_END);                         // move cursor to end
    size_t input_file_size = ftell(input_file);             // get position into var
    fseek(input_file, 0, input_file_size);                  // move cursor to start
    input_file_data = malloc(input_file_size);              // allocate buffer
    fread(input_file_data, 1, input_file_size, input_file); // read into buffer
    fclose(input_file);                                     // close file

    // create frequency pair array
    struct freq_interval_t* freqs_to_encode = malloc(sizeof(struct freq_interval_t) * input_file_size);
    for (int i = 0; i < input_file_size; i++) {
        int idx = find_idx_by_byte(map, ((char*)input_file_data)[i]);
        freqs_to_encode[i] = map[idx].interval;
    }
    free(input_file_data);

    struct sine_oscillator_t oscillator = sine_oscillator_create(0,     /* frequency */
                                                                 1,     /* amplitude */
                                                                 44100, /* sample_rate */
                                                                 16     /* bit_depth */);
    char* output_file_name = args.file;
    strcat(output_file_name, ".wav");
    FILE* output_file = fopen(output_file_name, "wb");
    if (!output_file) { printf("Failed to open output file\n"); return; }

    const int* __data_size = &(int){ oscillator.sample_rate * args.spf * input_file_size * oscillator.bit_depth / 8 };
    struct wav_header_t wav_header = {
        .RIFF = "RIFF",
        .size = 36 + *__data_size,
        .WAVE = "WAVE",
        .fmt = "fmt ",
        .fmt_size = 16,
        .fmt_type = 1,
        .channels = 1,
        .sample_rate = oscillator.sample_rate,
        .byte_rate = oscillator.sample_rate * oscillator.bit_depth / 8,
        .block_align = oscillator.bit_depth / 8,
        .bit_depth = oscillator.bit_depth,
        .data = "data",
        .data_size = *__data_size
    };
    fwrite(&wav_header, 1, sizeof(struct wav_header_t), output_file);

    for (int i = 0; i < input_file_size; i++) {
        for (int j = 0; j < oscillator.sample_rate * args.spf; j++) {
            float avg_freq = (freqs_to_encode[i].min + freqs_to_encode[i].max) / 2.0f;
            sine_oscillator_set_frequency(&oscillator, avg_freq);
            float sample = sine_oscillator_process(&oscillator);
            int sample_int = (int)(sample * oscillator.max_amplitude);
            fwrite(&sample_int, 1, 2 /* first two bytes of the sample */, output_file);
        }
    }

    fclose(output_file);
}