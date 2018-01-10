
#include <math.h>
#include "stdlib.h"
#include "waves.h"


WAVE_SIGNATURE(wave_sin) {
    return sinf(2 * M_PI * (t * hz));
}

WAVE_SIGNATURE(wave_saw) {
    return hz * (fmod(t, 2.0f / hz) - 1.0f / hz);
}

WAVE_SIGNATURE(wave_sqr) {
    return 2 * (fmod(t, 1.0f / hz) > ((.5f + tweak) / (hz))) - 1.0f;
}

WAVE_SIGNATURE(wave_tri) {
    return wave_sqr(t, hz, tweak) * wave_saw(t, hz, tweak);
}

WAVE_SIGNATURE(wave_noise) {
    return 2.0f * ((float)rand()/RAND_MAX) - 1.0f;
}


