#ifndef DECODE_H
#define DECODE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "args.h"
#include "freq_byte_map.h"
#include "wav_header.h"

void decode(struct args_t args);

#endif // DECODE_H