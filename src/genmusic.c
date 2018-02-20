
// generates music

#include "genmusic.h"
#include "noteman.h"

void str_from_note(char ** c, note_t note) {
    sprintf(*c, "%f,%f,%f,%f,%f,%d,%d,%f,%f,%f,%f", note.time_offset, note.duration, note.volume, note.tweak, note.wet, note.wave_function, note.semitone, note.A, note.D, note.S, note.R);
}

void note_from_str(note_t * note, char ** c) {
    sscanf(*c, "%f,%f,%f,%f,%f,%d,%d,%f,%f,%f,%f", &note->time_offset, &note->duration, &note->volume, &note->tweak, &note->wet, &note->wave_function, &note->semitone, &note->A, &note->D, &note->S, &note->R);
}

// depth is how many bits deep to modulate (1 through 32)

float rflt() {
    return (float)rand()/(float)RAND_MAX;
}


// depth is how many bits deep to modulate (1 through 32)

bool isvalidsemi(int semi) {
    int _s = semi % 12;
    return _s == 0 || _s == 2 || _s == 4 || _s == 5 || _s == 7 || _s == 9 || _s == 11;
}


void randomize_measure(note_t note, float sparse, int depth, int d_note) {
    //int ndelay = rand() & 0xffffffff, i;

    float start_off = note.time_offset;

    int start_semi = note.semitone;

    int i, nsemi;
    int ndur;

    for (i = 0; i < depth; ) {
        if (rflt() < sparse) {
            note.time_offset = start_off + 4.0f * (i * BEAT) / (depth);
            ndur = (int)(rflt() * 3 + 1);
            note.duration = ndur * 4.0f * BEAT / depth;
            if (d_note != 0) {
                do {
                    nsemi = (int)((rflt() - .5) * d_note);
                } while (!isvalidsemi(nsemi + note.semitone));
                note.semitone = start_semi + nsemi;
            }
            add_note(note);
            i += ndur;
            
        } else { i++; }
    }
}



void update_notes() {
    int i, ct = 0;
    float max_time_off = 0.0f;
    for (i = 0; i < MAX_NUM_NOTES; ++i) {
        if (notes_enabled[i]) {
            ct ++;
            if (cur_notes[i].time_offset > max_time_off) {
                max_time_off = cur_notes[i].time_offset;
            }
        }
    }
    if (ct * 2 < MAX_NUM_NOTES) {
        // now actually add notes
        note_t cnote;
        for (i = 0; num_notes_enabled < MAX_NUM_NOTES; ++i) {
            // add random snare notes

            int base = 10;

            // bass
            cnote = default_note;
            cnote.wave_function = WAVE_SIN;
            cnote.semitone = base;
            cnote.volume = .6f;
            cnote.time_offset = max_time_off + 4.0 * i * BEAT;
            randomize_measure(cnote, .5, 8, 10);

            /*

            // trap bass
            cnote = default_note;
            //cnote.wave_function = WAVE_TRI;
            cnote.wave_function = WAVE_808;
            cnote.semitone = 8;
            cnote.wet = 1.0;
            cnote.volume = 1.0f;
            cnote.time_offset = max_time_off + 4.0 * i * BEAT;
            randomize_measure(cnote, .75, 4, 6);
            */

            cnote = snare_note;
            cnote.volume = .2f;
            cnote.time_offset = max_time_off + 4.0 * i * BEAT;
            randomize_measure(cnote, .25, 8, 0);


            cnote = hat_note;
            cnote.volume = .1f;
            cnote.time_offset = max_time_off + 4.0 * i * BEAT;
            randomize_measure(cnote, .4, 24, 0);

            cnote = default_note;
            cnote.wave_function = WAVE_SAW;
            cnote.semitone = base + 3 * 12;
            cnote.wet = 1.0;
            cnote.volume = .2f;
            cnote.time_offset = max_time_off + 4.0 * i * BEAT;
            randomize_measure(cnote, .8, 8, 24);
    /*
            cnote = default_note;
            cnote.volume = .4f;
            cnote.wave_function = WAVE_SQR;
            cnote.semitone = 34;
            cnote.time_offset = max_time_off + 4.0 * i * BEAT;
            randomize_measure(cnote, 16);
*/
            //randomize_measure(cnote, 10);
            //randomize_measure(cnote, 5);
            //randomize_measure(cnote, 4);

        }
    }
}


