#include "encode.h"
#include "decode.h"

int main(int argc, char** argv)
{
    // usage: program.exe 0/1 (0 = encode, 1 = decode) seconds_per_frequency file
    if (argc < 4) { printf("Not enough arguments\n"); return -1; }
    struct args_t args = { atoi(argv[2]) /* spf */, argv[3] /* file */};

    switch (atoi(argv[1]) /* mode */) {
    case 0:
        encode(args);
        break;
    case 1:
        decode(args);
        break;
    default:
        printf("Invalid mode\n");
        break;
    }
}