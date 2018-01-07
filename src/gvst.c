
// my ghetto vst implementation

#include "tinymusicbox.h"
#include "gvst.h"

void gvst_copy(float * wet, float * dry) {
    int i;
    for (i = 0; i < FRAMES_PER_BUFFER; ++i) {
        wet[i] = dry[i];
    }
}



