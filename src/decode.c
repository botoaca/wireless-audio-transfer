#include "decode.h"

void decode(struct args_t args) {
    FILE* input_file = fopen(args.file, "rb");
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

    // remove first sizeof(wav_header_t) bytes to leave only the audio data
    input_file_data = (void*)((char*)input_file_data + sizeof(struct wav_header_t));
    input_file_size -= sizeof(struct wav_header_t);

    // todo: fast fourier transform
}