WIP!! not working with microphone transfer yet

# Wireless Audio Transfer
This project's purpose is to be able to convert binary files into waveform data to be played into the microphone of another device to be decoded from waveform data back into it's original binary format, thus making possbile the transfer of file through audio.

## Building
1. `mkdir build`
2. `make`

## Usage
### Encoding
1. `./build/output <mode: 0 (encode)> <seconds_per_frequency> <file.*>`, output: file transformed to audio
### Decoding
1. `./build/output <mode: 1 (decode)> <seconds_per_frequency> <file.wav>`, output: original file

## UI
To prepare the web UI, run the `prepare_ui.ps1` script on Windows. The commands inside that script are the same as POSIX ones, so you can just run `cat prepare_ui.ps1 > prepare_ui && chmod +x prepare_ui && ./prepare_ui` to run it on Linux/MacOS.