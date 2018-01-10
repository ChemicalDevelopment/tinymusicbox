
// envelope code
#include "envelope.h"

// t is relative to start of envelope
float envelope_factor(note_t note, float t) {
    if (t <= note.A) {
        return t / note.A;
    } else if (t <= note.A + note.D) {
        float cfact = (t - note.A) / note.D;
        return 1.0f - cfact * (1.0f - note.S);
    } else if (t <= note.duration) {
        return note.S;
    } else if (t <= note.duration + note.R) {
        float cfact = (t - note.duration) / note.R;
        return note.S * (1.0f - cfact);
    } else {
        return 0.0f;
    }
}
