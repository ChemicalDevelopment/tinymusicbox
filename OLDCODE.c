
#include <stdio.h>
#include <stdbool.h>

#include <math.h>

#include "portaudio.h"
 
// semitone value for A0 (27.5 hz) from 1 hz.
#define __SEMITONE_OFFSET (57.37631656229592f)


long total_frames_processed;

 


 /* This routine will be called by the PortAudio engine when audio is needed.
 ** It may called at interrupt level on some machines so don't do anything
 ** that could mess up the system like calling malloc() or free().
 */
static int patestCallback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData
) {

    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned long i, j;

    // how much time since we started playing music
    float total_time_offset;
    // offset in this buffer
    float local_time_offset;
    // offset in the current note
    float note_time_offset;

    float _val, val;
    float wave_val;
    float note_hz;

    cleanup_notes();
     
    for (i = 0; i < framesPerBuffer; i++) {
         total_time_offset = (float)total_frames_processed / SAMPLE_RATE;
         local_time_offset = (float)i / SAMPLE_RATE;

         _val = 0.0f;

         for (j = 0; j < MAX_NUM_NOTES; ++j) {
             if (notes_enabled[j] && total_time_offset >= cur_notes[j].time_offset && total_time_offset <= cur_notes[j].time_offset + cur_notes[j].duration) {
                note_time_offset = total_time_offset - cur_notes[j].time_offset;
                note_hz = hz_from_semitone(cur_notes[j].semitone);

                switch (cur_notes[j].wave_function) {
                    case WAVE_SAW:
                        wave_val = wave_saw(note_time_offset, note_hz);
                        break;
                    case WAVE_TRI:
                        wave_val = wave_tri(note_time_offset, note_hz);
                        break;
                    case WAVE_SQR:
                        wave_val = wave_sqr(note_time_offset, note_hz);
                        break;
                    case WAVE_SIN:
                    default:
                        wave_val = wave_sin(note_time_offset, note_hz);
                        break;
                }

                 val = cur_notes[j].volume * wave_val;
                 _val += val;
             }
         }

         //val = wave_tri(total_time_offset, 440);

         //printf("%f,", val);
         *out++ = _val;
         *out++ = _val;
         total_frames_processed++;
     }
     
     return paContinue;
 }
 
 /*
  * This routine is called by portaudio when playback is done.
  */
 static void StreamFinished( void* userData )
 {
    paTestData *data = (paTestData *) userData;
 }
 

int main(void) {
    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    paTestData data;
    int i;
 
    err = Pa_Initialize();
    if (err != paNoError) goto error;
 
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */

    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        goto error;
    }

    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL; 

    total_frames_processed = 0;
    
    for (i = 0; i < MAX_NUM_NOTES; ++i) {
        notes_enabled[i] = false;
    }


    note_t test_note;

    test_note.time_offset = 0.f;
    test_note.duration = 5.f;

    test_note.semitone = 48;
    test_note.volume = 1.0f;

    test_note.wave_function = WAVE_SAW;

    add_note(test_note);


    err = Pa_OpenStream(
        &stream,
        NULL, /* no input */
        &outputParameters,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff,      /* we won't output out of range samples so don't bother clipping them */
        patestCallback,
        &data
    );
    if (err != paNoError) goto error;
 
    err = Pa_SetStreamFinishedCallback(stream, &StreamFinished);
    if (err != paNoError) goto error;
 
    err = Pa_StartStream(stream);
    if (err != paNoError) goto error;
 
    while(1);
 //   Pa_Sleep(NUM_SECONDS * 1000);
 
    err = Pa_StopStream(stream);
    if (err != paNoError) goto error;
 
    err = Pa_CloseStream(stream);
    if (err != paNoError) goto error;
 
    Pa_Terminate();
    printf("Test finished.\n");
     
}

