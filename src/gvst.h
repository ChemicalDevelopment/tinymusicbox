
// my ghetto vst implementation

#ifndef GVST_H
#define GVST_H


void gvst_copy(float * wet, float * dry);

void gvst_clipper(float * wet, float * dry, float clipmark);

void gvst_lowpass(float * wet, float * dry, float cutoff);

#endif

