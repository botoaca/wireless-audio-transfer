#include "sine_oscillator.h"
#define _USE_MATH_DEFINES
#include <math.h>

struct sine_oscillator_t sine_oscillator_create(float frequency, float amplitude, int sample_rate, int bit_depth) {
    struct sine_oscillator_t ret;
    ret.frequency = frequency;
    ret.amplitude = amplitude;
    ret.angle = 0.0f;
    ret.sample_rate = sample_rate;
    ret.bit_depth = bit_depth;
    ret.max_amplitude = pow(2, bit_depth - 1) - 1;
    ret.offset = 2 * M_PI * frequency / sample_rate;
    return ret;
}

void sine_oscillator_set_frequency(struct sine_oscillator_t* oscillator, float frequency) {
    oscillator->frequency = frequency;
    oscillator->angle = 0.0f;
    oscillator->offset = 2 * M_PI * frequency / oscillator->sample_rate;
}

float sine_oscillator_process(struct sine_oscillator_t* oscillator) {
    float sample = oscillator->amplitude * sin(oscillator->angle);
    oscillator->angle += oscillator->offset;
    return sample;
}