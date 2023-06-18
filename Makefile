C = gcc
CFLAGS = -Wall -Iinclude
FFTFLAGS = -Ikissfft

all: build build/output

build:
	mkdir build

build/output: build/main.o build/freq_byte_map.o build/sine_oscillator.o build/encode.o build/decode.o build/kissfft.o
	$(C) $(CFLAGS) $^ -o $@

build/main.o: src/main.c
	$(C) $(CFLAGS) $(FFTFLAGS) -c $< -o $@

build/freq_byte_map.o: src/freq_byte_map.c
	$(C) $(CFLAGS) -c $< -o $@

build/sine_oscillator.o: src/sine_oscillator.c
	$(C) $(CFLAGS) -c $< -o $@

build/encode.o: src/encode.c
	$(C) $(CFLAGS) -c $< -o $@

build/decode.o: src/decode.c
	$(C) $(CFLAGS) $(FFTFLAGS) -c $< -o $@

build/kissfft.o: kissfft/kiss_fft.c
	$(C) $(CFLAGS) $(FFTFLAGS) -c $< -o $@