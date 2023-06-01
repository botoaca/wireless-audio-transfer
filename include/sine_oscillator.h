#ifndef SINE_OSCILLATOR_H
#define SINE_OSCILLATOR_H

struct sine_oscillator_t {
    float frequency, amplitude, angle, offset, max_amplitude;
    int sample_rate, bit_depth;
};

struct sine_oscillator_t sine_oscillator_create(float frequency, float amplitude, int sample_rate, int bit_depth);
void sine_oscillator_set_frequency(struct sine_oscillator_t* oscillator, float frequency);
float sine_oscillator_process(struct sine_oscillator_t* oscillator);

#endif // SINE_OSCILLATOR_H