
// generates music

#include "genmusic.h"
#include "noteman.h"

void str_from_note(char ** c, note_t note) {
    sprintf(*c, "%f,%f,%f,%f,%f,%d,%d,%f,%f,%f,%f", note.time_offset, note.duration, note.volume, note.tweak, note.wet, note.wave_function, note.semitone, note.A, note.D, note.S, note.R);
}

void note_from_str(note_t note, char ** c) {
    sscanf(*c, "%f,%f,%f,%f,%f,%d,%d,%f,%f,%f,%f", &note.time_offset, &note.duration, &note.volume, &note.tweak, &note.wet, &note.wave_function, &note.semitone, &note.A, &note.D, &note.S, &note.R);
}

// depth is how many bits deep to modulate (1 through 32)

void randomize_measure_beat(note_t note, int depth) {
    int ndelay = rand() & 0xffffffff, i;

    float start_off = note.time_offset;
    
    for (i = 0; i < depth; ++i) {
        if (ndelay & (1 << i)) {
            note.time_offset = start_off + 4.0f * (i * BEAT) / (depth);
            add_note(note);
        }
    }
}

// depth is how many bits deep to modulate (1 through 32)

void randomize_measure(note_t note, int depth) {
    int i;

    int ndelay = rand() & 0xffffffff;

    int nsemi;

    float start_off = note.time_offset;

    int start_semitone = note.semitone;
    
    note.duration = 4 * BEAT / depth;

    for (i = 0; i < depth; ++i) {
        if (ndelay & (1 << i)) {
            note.time_offset = start_off + 4.0f * (i * BEAT) / (depth);
            nsemi = rand() % 16 - 8;
            note.semitone = start_semitone + nsemi;
            add_note(note);
        }
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

            // bass
            /*
            cnote = default_note;
            cnote.wave_function = WAVE_SQR;
            cnote.semitone = 5;
            cnote.volume = .2f;
            cnote.time_offset = max_time_off + 4.0 * i * BEAT;
            randomize_measure_beat(cnote, 8);

            */

            cnote = snare_note;
            cnote.volume = .4f;
            cnote.time_offset = max_time_off + 4.0 * i * BEAT;
            randomize_measure_beat(cnote, 8);

            cnote = hat_note;
            cnote.volume = .76f;
            cnote.time_offset = max_time_off + 4.0 * i * BEAT;
            randomize_measure_beat(cnote, 16);

            cnote = default_note;
            cnote.wave_function = WAVE_TRI;
            cnote.semitone = 21;
            cnote.time_offset = max_time_off + 4.0 * i * BEAT;
            randomize_measure(cnote, 8);

            cnote = default_note;
            cnote.volume = .7f;
            cnote.wave_function = WAVE_SAW;
            cnote.semitone = 52;
            cnote.time_offset = max_time_off + 4.0 * i * BEAT;
            randomize_measure(cnote, 16);

        }
    }
}


