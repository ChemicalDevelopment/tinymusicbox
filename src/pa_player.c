
// portaudio hookin and player

#include <portaudio.h>


PaStream * pa_stream;
PaStreamParameters pa_parameters;
PaError pa_error;


void tmb_pa_init() {
    pa_parameters.device = Pa_GetDefaultOutputDevice(); /* default output device */

    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        goto error;
    }

}

void tmb_pa_errorhandle() {
    Pa_Terminate();
    fprintf(stderr, "An error occured while using the portaudio stream\n");
    fprintf(stderr, "Error[%d]: %s\n", pa_error, Pa_GetErrorText(pa_error));
    exit(pa_error);
}


// callback audio function for generating sample data
static int tmb_pa_callback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData
) {


}


