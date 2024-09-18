#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <portaudio.h>
#include "/opt/homebrew/Cellar/portaudio/19.7.0/include/portaudio.h"
#include "/opt/homebrew/Cellar/libsndfile/1.2.2/include/sndfile.h"
#include <ncurses.h>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 256
#define CHANNELS 1

int main(int argc, char *argv[]){
    printf("Hello, World!\n");
    return 0;
}


typedef struct {
    SNDFILE *file;
    SF_INFO sfinfo;
} AudioData;


// static int recordCallback(const void *input, void *output, unsigned long frameCount,    const PaStreamCallbackTimeInfo *timeInfo,
// PaStreamCallbackFlags statusFlags, void *userData) {
//     // count = sf_writef_float (sndfile, obuf, num_output_frames);

// }


static int recordCallback(const void *input, void *output,
                          unsigned long frameCount,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData) {
    AudioData *audioData = (AudioData*)userData;
    const short *in = (const short*)input;

    // Write the input audio data to the file
    if (sf_writef_short(audioData->file, in, frameCount) != frameCount) {
        printf("Error writing to file\n");
        return paComplete;  // End the stream on error
    }

    return paContinue;  // Continue processing audio
}