
#include <math.h>
#include "stdlib.h"
#include "waves.h"


WAVE_SIGNATURE(wave_sin) {
    float bs = sinf(2 * M_PI * (t * hz));
    return bs;
    //int sgn = (bs > 0) ? 1 : -1;
    //return sgn * powf(fabs(bs), tweak + 1);
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

WAVE_SIGNATURE(wave_808) {
    float sfact = 7 * (tweak + 1) * t + 0.5;
    if (sfact < 1) {
        hz /= sfact;
    }
    float bs = wave_sin(t, hz, tweak);// * wave_saw(t, hz, tweak);
    float ctoff = .3;
    if (bs > ctoff) bs = ctoff;
    if (bs < -ctoff) bs = -ctoff;
    return bs / ctoff;
    
}




