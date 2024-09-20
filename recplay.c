#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <portaudio.h>
// #include <sndfile.h>
#include "/opt/homebrew/Cellar/portaudio/19.7.0/include/portaudio.h"
#include "/opt/homebrew/Cellar/libsndfile/1.2.2/include/sndfile.h"
#include <ncurses.h>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 256
#define CHANNELS 1

typedef struct {
    SNDFILE *file;
    SF_INFO sfinfo;
} AudioData;

static int recordCallback(const void *input, void *output,
                          unsigned long frameCount,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData) {
                            
    AudioData *audioData = (AudioData*)userData;
    short *in = ( short*)input;
    // short *out = (short*)output;

    if (sf_writef_short(audioData->file, in, frameCount) != frameCount) {
        printf("Error writing to file\n");
        return paComplete;  
    }
    return paContinue;
}


static int playCallBack(const void *input, void *output,
                        unsigned long frameCount,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData) {
    AudioData *audioData = (AudioData*)userData;
    short *in = ( short*)input;
    short *out = (short*)output;

    for (int i = 0; i < frameCount - 1 ; i++) {
        out[i] = in[i];
    }
    if (sf_readf_short(audioData->file, out, frameCount) != frameCount) {
        printf("Error reading from file\n");
        return paComplete;
    }

    return paContinue;
}

void print_device_info(const PaDeviceInfo *deviceInfo) {
    printf("Device Name: %s\n", deviceInfo->name);
    printf("Max Input Channels: %d\n", deviceInfo->maxInputChannels);
    printf("Max Output Channels: %d\n", deviceInfo->maxOutputChannels);
}

int main(int argc, char *argv[]){
    PaError err;
    PaStream *stream;
    AudioData audioData;
    memset(&audioData.sfinfo, 0, sizeof(SF_INFO));
    audioData.sfinfo.samplerate = SAMPLE_RATE;
    audioData.sfinfo.channels = CHANNELS;
    audioData.sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    if (!audioData.file) {
            printf("Error opening file for recording: %s\n", sf_strerror(NULL));
            return 1;
        } else {
        // Playback mode: open the file for reading
        audioData.file = sf_open(argv[2], SFM_READ, &audioData.sfinfo);
        if (!audioData.file) {
            printf("Error opening file for playback: %s\n", sf_strerror(NULL));
            return 1;
        }
    }
    
    // initscr();
    // noecho();
    // getch();
    // endwin();
    return 0;
}