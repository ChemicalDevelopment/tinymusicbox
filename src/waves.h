
#ifndef WAVES_H
#define WAVES_H

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

#define WAVE_SIGNATURE(fname) float fname (float t, float hz, float tweak)


WAVE_SIGNATURE(wave_sin);

WAVE_SIGNATURE(wave_saw);

WAVE_SIGNATURE(wave_sqr);

WAVE_SIGNATURE(wave_tri);

WAVE_SIGNATURE(wave_noise);


#endif
