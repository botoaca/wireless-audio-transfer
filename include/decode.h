#ifndef DECODE_H
#define DECODE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "util.h"
#include "args.h"
#include "freq_byte_map.h"
#include "wav_header.h"

#include "kiss_fft.h"

void decode(struct args_t args);

#endif // DECODE_H