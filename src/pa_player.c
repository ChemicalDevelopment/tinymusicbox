
// portaudio hookin and player

#include <portaudio.h>

#include "pa_player.h"
#include "noteman.h"
#include "tinymusicbox.h"
#include "gvst.h"

unsigned long long total_frames;

PaStream * pa_stream;
PaStreamParameters pa_parameters;
PaError pa_error;



float * last_dry_signal;
float ** last_dry_signal_arr;

float * last_frame;


// tmp variables

float * cur_instrument;


float * dry_signal, * wet_signal;


void tmb_pa_init() {

    PA_HANDLE(Pa_Initialize());

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
        tmb_pa_callback,
        &data
    ));

    last_frame = malloc(sizeof(float) * FRAMES_PER_BUFFER);
    dry_signal = malloc(sizeof(float) * FRAMES_PER_BUFFER);
    wet_signal = malloc(sizeof(float) * FRAMES_PER_BUFFER);
    cur_instrument = malloc(sizeof(float) * FRAMES_PER_BUFFER);
    

    last_dry_signal = malloc(sizeof(float) * FRAMES_PER_BUFFER);
    last_dry_signal_arr = malloc(sizeof(float *) * MAX_NUM_NOTES);

    int i;
    for (i = 0; i < MAX_NUM_NOTES; ++i) {
        last_dry_signal_arr[i] = malloc(sizeof(float) * FRAMES_PER_BUFFER);
    }
    

    PA_HANDLE(Pa_SetStreamFinishedCallback(pa_stream, &tmb_pa_streamfinished));

    PA_HANDLE(Pa_StartStream(pa_stream));

}

void tmb_pa_stop() {
    PA_HANDLE(Pa_StopStream(pa_stream));
    PA_HANDLE(Pa_CloseStream(pa_stream));
    Pa_Terminate();

    free(dry_signal);
    free(wet_signal);
    free(cur_instrument);
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
    int i, j, k;

    // global (since sample 0), and local (since this callback function)
    float global_time, note_time;

    float val;

    float wet_param;


    // current note
    note_t cnote;

    cleanup_notes((float)total_frames / SAMPLE_RATE);

    // update if any new ones are generated
    update_notes();



    for (i = 0; i < framesPerBuffer; ++i) {
        for (k = 0; k < CHANNELS; ++k) {
            out[CHANNELS * i + k] = 0.0f;
        }
    }

    for (j = 0; j < MAX_NUM_NOTES; ++j) {
        cnote = cur_notes[j];
        
        if (notes_enabled[j]) {
            memset(cur_instrument, 0x00, CHANNELS * FRAMES_PER_BUFFER * sizeof(float));

            for (i = 0; i < framesPerBuffer; ++i) {
                global_time = (float)(total_frames + i) / SAMPLE_RATE;
                note_time = global_time - cnote.time_offset;
                cur_instrument[i] = eval_note(cnote, note_time);
            }

            //printf("%f ", cur_instrument[0]);

            memcpy(last_dry_signal, last_dry_signal_arr[j], sizeof(float) * FRAMES_PER_BUFFER);

            memcpy(dry_signal, cur_instrument, sizeof(float) * FRAMES_PER_BUFFER);
            memcpy(wet_signal, dry_signal, sizeof(float) * FRAMES_PER_BUFFER);

            memcpy(last_dry_signal_arr[j], dry_signal, sizeof(float) * FRAMES_PER_BUFFER);

            // do gvst processing here

            //gvst_copy(wet_signal, dry_signal);
            //gvst_lowpass(wet_signal, dry_signal, 20000);
            gvst_clipper(wet_signal, dry_signal, .4f);
            gvst_boost(wet_signal, dry_signal, 2.5f);
            gvst_flanger(wet_signal, dry_signal, 20, .33f);

            wet_param = cnote.wet;
            if (wet_param < 0.0f) wet_param = 0.0f;
            if (wet_param > 1.0f) wet_param = 1.0f;

            for (i = 0; i < framesPerBuffer; ++i) {
                val = wet_param * wet_signal[i] + (1.0f - wet_param) * dry_signal[i];
                
                for (k = 0; k < CHANNELS; ++k) {
                    out[CHANNELS * i + k] += val;
                }
            }
        }
    }

    for (i = 0; i < framesPerBuffer; ++i) {
        last_frame[i] = out[i];
    }

    total_frames += framesPerBuffer;

    return paContinue;
}


