#include "decode.h"

void decode(struct args_t args) {
    int max_calculated_freq = 0;
    struct freq_byte_pair_t* map = freq_byte_map_create(10, /* offset */
                                                        10, /* start_freq */
                                                        &max_calculated_freq);

    FILE* input_file = fopen(args.file, "rb");              // open file
    void* input_file_data = NULL;

    if (!input_file) { printf("Failed to open input file\n"); return; }
    fseek(input_file, 0, SEEK_END);                         // move cursor to end
    size_t input_file_size = ftell(input_file);             // get position into var
    fseek(input_file, 0, SEEK_SET);                         // move cursor to start
    input_file_data = malloc(input_file_size);              // allocate buffer
    fread(input_file_data, 1, input_file_size, input_file); // read into buffer
    fclose(input_file);                                     // close file

    // store the input file's header in a wav_header_t
    struct wav_header_t input_file_wav_header;
    memcpy(&input_file_wav_header, input_file_data, sizeof(struct wav_header_t));

    // remove header and cast to uint16_t
    int16_t* raw_audio_samples = (int16_t*)(input_file_data);
    raw_audio_samples += sizeof(struct wav_header_t);
    input_file_size -= sizeof(struct wav_header_t);

    int input_sample_rate = input_file_wav_header.sample_rate;
    int fft_size = input_sample_rate;
    int num_bins = fft_size / 2 + 1;
    int num_seconds = input_file_size / input_file_wav_header.block_align / input_file_wav_header.sample_rate;

    // FFT
    int* final_frequencies = malloc(num_seconds * sizeof(int));
    for (int i = 0; i < num_seconds; i++) {
        int start_position = i * input_sample_rate;

        // init kiss_fft
        kiss_fft_cfg cfg = kiss_fft_alloc(fft_size, 0, NULL, NULL);
        kiss_fft_cpx* fft_input = malloc(fft_size * sizeof(kiss_fft_cpx));
        kiss_fft_cpx* fft_output = malloc(fft_size * sizeof(kiss_fft_cpx));

        // populate fft_input array
        for (int j = 0; j < fft_size; j++) {
            fft_input[j].r = raw_audio_samples[start_position + j] / 32768.0 /* max of int16_t */;
            fft_input[j].i = 0.0;
        }

        kiss_fft(cfg, fft_input, fft_output);

        // extract the magnitude of each freq bin
        float* magnitudes = malloc(num_bins * sizeof(float));
        for (size_t j = 0; j < num_bins; j++) {
            float re = fft_output[j].r;
            float im = fft_output[j].i;
            float magnitude = sqrtf(re * re + im * im);
            magnitudes[j] = magnitude;
        }
        
        // find the max freq for each second
        float max_magnitude = 0.0;
        int max_magnitude_idx = 0;
        for (int j = 0; j < num_bins; j++) {
            if (magnitudes[j] > max_magnitude) {
                max_magnitude = magnitudes[j];
                max_magnitude_idx = j;
            }
        }
        float max_freq = (float)max_magnitude_idx * (float)input_sample_rate / (float)fft_size;
        final_frequencies[i] = (int)max_freq;

        free(fft_input);
        free(fft_output);
        free(cfg);
    }

    // find the byte for each frequency
    unsigned char* final_bytes = malloc(num_seconds * sizeof(unsigned char));
    for (int i = 0; i < num_seconds; i++) {
        int idx = find_idx_by_freq(map, final_frequencies[i]);
        final_bytes[i] = map[idx].byte;
    }

    // extract output filename size
    int output_filename_size = final_bytes[0];
    final_bytes += 1;

    // extract output filename
    char* output_filename = malloc(output_filename_size + 1);
    memcpy(output_filename, final_bytes, output_filename_size);
    output_filename[output_filename_size] = '\0';
    final_bytes += output_filename_size;

    // write output file
    FILE* output_file = fopen(output_filename, "wb");
    fwrite(final_bytes, 1, num_seconds - output_filename_size - 1, output_file);
    fclose(output_file);
}