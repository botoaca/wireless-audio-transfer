#ifndef ENCODE_H
#define ENCODE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
#include "args.h"
#include "freq_byte_map.h"
#include "sine_oscillator.h"
#include "wav_header.h"

void encode(struct args_t args);

#endif // ENCODE_H