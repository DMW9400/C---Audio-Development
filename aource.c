#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define SAMPLE_RATE 44100
#define AMPLITUDE 32767
#define PI 3.14159265358979

// Write WAV header with minimal fields
// Function is expecting a pointer to a file of type FILE, a 'struct' in one of the included libraries 
// Pointers eliminate need to move data around unneccessarily by pointing to place in memory
void write_wav_header(FILE *file, int sample_rate, int num_samples) {
    int32_t chunk_size = 36 + num_samples * sizeof(int16_t);
    // fwrite writes data to the file stream pointed to by file
    // it is a kind of append function, adding data to the end of the file
    fwrite("RIFF", 1, 4, file);
    // The first 4 bytes of any WAV file need to be RIFF
    fwrite(&chunk_size, 4, 1, file);
    fwrite("WAVEfmt ", 1, 8, file);
    int32_t subchunk1_size = 16;
    int16_t audio_format = 1, num_channels = 1, bits_per_sample = 16;
    int32_t byte_rate = sample_rate * num_channels * bits_per_sample / 8;
    fwrite(&subchunk1_size, 4, 1, file);
    fwrite(&audio_format, 2, 1, file);
    fwrite(&num_channels, 2, 1, file);
    fwrite(&sample_rate, 4, 1, file);
    fwrite(&byte_rate, 4, 1, file);
    int16_t block_align = num_channels * bits_per_sample / 8;
    fwrite(&block_align, 2, 1, file);
    fwrite(&bits_per_sample, 2, 1, file);
    fwrite("data", 1, 4, file);
    int32_t subchunk2_size = num_samples * sizeof(int16_t);
    fwrite(&subchunk2_size, 4, 1, file);
}

#define Attack = .2;
// Q: How could I apply an attack envelope to this sound?
// A: You could multiply the amplitude of the sound by a function that increases from 0 to 1 over the first 20% of the sound.
// Q: could you write this out in C Code?
// A: Sure, here is an example of how you could apply an attack envelope to the sound:  
// int16_t sample = (int16_t)(AMPLITUDE * sin(2.0 * PI * frequency * i / SAMPLE_RATE) * (i / (SAMPLE_RATE * Attack)));
// This code multiplies the amplitude of the sound by a value that increases from 0 to 1 over the first 20% of the sound.

// Generate a sine wave for a note's frequency and duration
void generate_sine_wave(FILE *file, float frequency, int duration_ms) {
    int num_samples = (int)((duration_ms / 1000.0) * SAMPLE_RATE);
    for (int i = 0; i < num_samples; i++) {
        int16_t sample = (int16_t)(AMPLITUDE * sin(2.0 * PI * frequency * i / SAMPLE_RATE));
        // Q: What is the purpose of the & below?
        // A: The & is a bitwise AND operator, it is used to get the address of the variable
        fwrite(&sample, sizeof(int16_t), 1, file);
    }
}

// Generate silence for rests or gaps
void generate_silence(FILE *file, int duration_ms) {
    int num_samples = (int)((duration_ms / 1000.0) * SAMPLE_RATE);
    int16_t silence = 0;
    for (int i = 0; i < num_samples; i++) {
        fwrite(&silence, sizeof(int16_t), 1, file);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input text file> <output wav file>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    // Returns a pointer to the file stream
    FILE *output_file = fopen(argv[2], "wb");
    if (!input_file || !output_file) {
        perror("File error");
        return 1;
    }

    char line[100];
    int total_samples = 0;

    // First pass: Calculate total number of samples
    while (fgets(line, sizeof(line), input_file)) {
        float frequency;
        int start_time, duration;
        sscanf(line, "%f,%d,%d", &frequency, &start_time, &duration);
        total_samples += (int)((duration / 1000.0) * SAMPLE_RATE);
    }

    // Write WAV header
    write_wav_header(output_file, SAMPLE_RATE, total_samples);
    fseek(input_file, 0, SEEK_SET);  // Reset file pointer

    int previous_end_time = 0;

    // Second pass: Generate WAV data
    while (fgets(line, sizeof(line), input_file)) {
        float frequency;
        int start_time, duration;
        sscanf(line, "%f,%d,%d", &frequency, &start_time, &duration);

        if (start_time > previous_end_time) {
            generate_silence(output_file, start_time - previous_end_time);  // Insert silence if needed
        }

        if (frequency > 0) {
            generate_sine_wave(output_file, frequency, duration);  // Generate note
        } else {
            generate_silence(output_file, duration);  // Handle rests
        }

        previous_end_time = start_time + duration;
    }

    fclose(input_file);
    fclose(output_file);

    printf("WAV file generated successfully.\n");
    return 0;

}
