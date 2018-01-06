
// portaudio hookin and player

#include <portaudio.h>

#include "pa_player.h"
#include "noteman.h"
#include "tinymusicbox.h"

unsigned long long total_frames;

PaStream * pa_stream;
PaStreamParameters pa_parameters;
PaError pa_error;


void tmb_pa_init() {
    pa_parameters.device = Pa_GetDefaultOutputDevice(); /* default output device */

    total_frames = 0;

    if (pa_parameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        tmb_pa_errorhandle();
    }
    
    paTestData data;

    pa_parameters.channelCount = CHANNELS;       /* stereo output */
    pa_parameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    pa_parameters.suggestedLatency = Pa_GetDeviceInfo(pa_parameters.device)->defaultLowOutputLatency;
    pa_parameters.hostApiSpecificStreamInfo = NULL; 

    PA_HANDLE(Pa_OpenStream(
        &pa_stream,
        NULL, /* no input */
        &pa_parameters,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff,      /* we won't output out of range samples so don't bother clipping them */
        patestCallback,
        &data
    ));

    PA_HANDLE(Pa_SetStreamFinishedCallback(pa_stream, &tmp_pa_streamfinished));

    PA_HANDLE(Pa_StartStream(pa_stream));

}

void tmb_pa_stop() {
    PA_HANDLE(Pa_StopStream(pa_stream));
    PA_HANDLE(Pa_CloseStream(pa_stream));
    Pa_Terminate();
}

void tmb_pa_errorhandle() {
    Pa_Terminate();
    fprintf(stderr, "An error occured while using the portaudio stream\n");
    fprintf(stderr, "Error[%d]: %s\n", pa_error, Pa_GetErrorText(pa_error));
    exit(pa_error);
}


void tmb_pa_streamfinished(void *userData) {
    printf("stream finished\n");
}

// callback audio function for generating sample data
int tmb_pa_callback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData
) {
    // output buffer
    float *out = (float*)outputBuffer;

    // loop vars
    int i, j;

    // global (since sample 0), and local (since this callback function)
    float global_time, local_time, note_time;

    // current note
    note_t cnote;

    float note_val, total_val;

    for (i = 0; i < framesPerBuffer; ++i) {
        global_time = (float)total_frames / SAMPLE_RATE;
        local_time = (float)i / SAMPLE_RATE;

        total_val = 0.0f;
        
        for (j = 0; j < MAX_NUM_NOTES; ++j) {
            cnote = cur_notes[j];
            if (notes_enabled[j] && global_time >= cnote.time_offset && global_time <= cnote.time_offset + cnote.duration) {
                note_time = global_time - cnote.time_offset;
                note_val = eval_note(cnote, note_time);
                total_val += note_val;
            }
        }

        *out++ = total_val;
        *out++ = total_val;
        total_frames++;

    }


    return paContinue;
}


