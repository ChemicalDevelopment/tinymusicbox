
// envelope code
#include "envelope.h"

// t is relative to start of envelope
float envelope_factor(note_t note, float t) {
    if (t <= note.A) {
        if (note.A == 0.0f) {
            return 0.0f;
        } else {
            return t / note.A;
        }
    } else if (t <= note.A + note.D) {
        if (note.D == 0.0f) {
            return note.S;
        } else {
            float cfact = (t - note.A) / note.D;
            float ret = 1.0f - cfact * (1.0f - note.S);
            return ret;
        }
    } else if (t <= note.duration) {
        return note.S;
    } else if (t <= note.duration + note.R) {
        if (note.R == 0.0f) {
            return 0.0f;
        } else {
            float cfact = (t - note.duration) / note.R;
            return note.S * (1.0f - cfact);
        }
    } else {
        return 0.0f;
    }
}
