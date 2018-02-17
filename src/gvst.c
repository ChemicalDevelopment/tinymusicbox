
// my ghetto vst implementation

#include "tinymusicbox.h"
#include "gvst.h"
#include "pa_player.h"
#include "waves.h"

// example function that does nothing

void gvst_copy(float * wet, float * dry) {
    int i;
    for (i = 0; i < FRAMES_PER_BUFFER; ++i) {
        wet[i] = dry[i];
    }
}


void gvst_boost(float * wet, float * dry, float p) {
    int i;
    for (i = 0; i < FRAMES_PER_BUFFER; ++i) {
        wet[i] = dry[i] * p;
    }
}

/*

lowpass filter (to make things sound less annoying)

*/

void gvst_lowpass(float * wet, float * dry, float cutoff) {
    float rc = 1.0f / (cutoff * 2 * M_PI);
    float dt = 1.0 / SAMPLE_RATE;
    float a = dt / (rc + dt);
    int i;
    wet[0] = last_frame[FRAMES_PER_BUFFER - 1] + a * (dry[0] - last_frame[FRAMES_PER_BUFFER - 1]);
    for (i = 1; i < FRAMES_PER_BUFFER; ++i) {
        wet[i] = dry[i - 1] + a * (dry[i] - dry[i - 1]);
    }
}


/*

simple clipper, clips all above +clipmark and below -clipmark

*/

void gvst_clipper(float * wet, float * dry, float clipmark) {
    clipmark = fabs(clipmark);
    int i;
    for (i = 0; i < FRAMES_PER_BUFFER; ++i) {
        if (fabs(dry[i]) > clipmark) {
            if (dry[i] > 0) {
                wet[i] = clipmark;
            } else {
                wet[i] = -clipmark;
            }
        } else {
            wet[i] = dry[i];
        }
    }
}


void gvst_flanger(float * wet, float * dry, float ms, float hz) {
    int i, j;
    float tmp;
    for (i = 0; i < FRAMES_PER_BUFFER; ++i) {
        tmp = (1.0f + sinf(hz * 2 * M_PI * (float)(total_frames + i) / SAMPLE_RATE)) / 2.0f;
        j = i - (int)floor(ms * SAMPLE_RATE * tmp / 1000.0f + 0.5f);
        if (j >= 0 && j < FRAMES_PER_BUFFER) wet[i] = dry[i] + dry[j];
        else if (j + FRAMES_PER_BUFFER >= 0 && j + FRAMES_PER_BUFFER < FRAMES_PER_BUFFER) wet[i] = dry[i] + last_dry_signal[j + FRAMES_PER_BUFFER];
        else wet[i] = 2.0f * dry[i];
    }
}


