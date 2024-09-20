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
    short *out = (short*)output;

    if (sf_writef_short(audioData->file, in, frameCount) != frameCount) {
        printf("Error writing to file\n");
        return paComplete;  
    }
    return paContinue;
}


// static int playCallBack(const void *input, void *output,
//                         unsigned long frameCount,
//                         const PaStreamCallbackTimeInfo *timeInfo,
//                         PaStreamCallbackFlags statusFlags,
//                         void *userData) {
//     AudioData *audioData = (AudioData*)userData;
//     short *in = ( short*)input;
//     short *out = (short*)output;
//     // printf("playing callback");
//     for (int i = 0; i < frameCount - 1 ; i++) {
//         out[i] = in[i];
//     }
//     if (sf_readf_short(audioData->file, out, frameCount) != frameCount) {
//         printf("Error reading from file\n");
//         return paComplete;
//     }

//     return paContinue;
// }

static int playCallBack(const void *input, void *output,
                        unsigned long frameCount,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData) {
    AudioData *audioData = (AudioData*)userData;
    short *out = (short*)output;

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


int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <r|p> <filename.wav>\n", argv[0]);
        return 1;
    }

    // Initialize PortAudio
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    AudioData data;
    memset(&data, 0, sizeof(AudioData));
    
    // Parse command-line arguments
    int isRecording = (argv[1][0] == 'r');
    if (isRecording) {
        // Recording mode: open the file for writing
        data.sfinfo.samplerate = SAMPLE_RATE;
        data.sfinfo.channels = CHANNELS;
        data.sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
        data.file = sf_open(argv[2], SFM_WRITE, &data.sfinfo);
        if (!data.file) {
            printf("Error opening file for recording: %s\n", sf_strerror(NULL));
            return 1;
        }
    } else {
        // Playback mode: open the file for reading
        data.file = sf_open(argv[2], SFM_READ, &data.sfinfo);
        if (!data.file) {
            printf("Error opening file for playback: %s\n", sf_strerror(NULL));
            return 1;
        }
    }

    // Print device info
    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice());
    initscr();  // Start ncurses
    print_device_info(deviceInfo);

    // Open the PortAudio stream
    PaStream *stream;
    if (isRecording) {
        err = Pa_OpenDefaultStream(&stream, CHANNELS, 0, paInt16, SAMPLE_RATE,
                                   FRAMES_PER_BUFFER, recordCallback, &data);
    } else {
        err = Pa_OpenDefaultStream(&stream, 0, CHANNELS, paInt16, data.sfinfo.samplerate,
                                   FRAMES_PER_BUFFER, playCallBack, &data);
    }

    if (err != paNoError) {
        printw("Error opening PortAudio stream: %s\n", Pa_GetErrorText(err));
        endwin();
        Pa_Terminate();
        return 1;
    }

    // Start the stream
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        printw("Error starting PortAudio stream: %s\n", Pa_GetErrorText(err));
        endwin();
        Pa_Terminate();
        return 1;
    }

    // Handle user input and stream termination
    timeout(100);  // Set input timeout for ncurses
    int ch;
    while ((ch = getch()) != 'q' && ch != 'Q') {
        if (Pa_IsStreamActive(stream) == 0)
            break;
    }

    // Clean up resources
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    sf_close(data.file);
    Pa_Terminate();
    endwin();  // End ncurses

    return 0;
}