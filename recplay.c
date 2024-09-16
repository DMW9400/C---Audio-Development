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


static int recordCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo,
PaStreamCallbackFlags statusFlags, void *userData) {
// Your code here
}