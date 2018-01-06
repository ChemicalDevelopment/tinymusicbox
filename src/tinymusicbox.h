// main tinymusicbox header

#ifndef TINYMUSICBOX_H
#define TINYMUSICBOX_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdbool.h>

#include <portaudio.h>
#include <math.h>


#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (128)
#define CHANNELS (2)

int main(int argc, char ** argv);


#endif
