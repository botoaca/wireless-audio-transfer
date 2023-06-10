# This branch is W.I.P.

Currently, the way I encode/decode frequencies leaves room for errors, where, for example, an encoded 'a' could be decoded as 'b' due to this line in `encode.c`: `int sample_int = (int)(sample * oscillator.max_amplitude);`.