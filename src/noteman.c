
// note manager

#include "noteman.h"


// which notes are being played
note_t cur_notes[MAX_NUM_NOTES];
bool notes_enabled[MAX_NUM_NOTES];

// the default value (offset, duration, volume, tweak, wave_function, semitone)
note_t default_note = { 0.0f, 4.0f, 1.0f, 0.0f, WAVE_SIN, 48 };


// note manipulation functions

// adds a note to active notes
void add_note(note_t note) {
    int i;
    for (i = 0; i < MAX_NUM_NOTES; ++i) {
        if (!notes_enabled[i]) {
            cur_notes[i] = note;
            notes_enabled[i] = true;
            return;
        }
    }
    printf("WARNING: too many notes, note was not added\n");
}

// sets a note to inactive
void remove_note(int note_id) {
    if (note_id >= 0 && note_id < MAX_NUM_NOTES) {
        notes_enabled[note_id] = false;
    }
}

// macro value for integer semitone offsets
#define __SEMITONE_OFFSET_A0 (57.37631656229592f)

// hz in semitone from A0, so A1 should be semitone=12, A4 means semitone=48
float hz_from_semitone(int semitone) {
    return pow(2.0f, (semitone + __SEMITONE_OFFSET_A0) / 12.0f);
}

// returns the sample of the note at time 't'
float eval_note(note_t note, float t) {
    if (t >= note.time_offset && t <= note.time_offset + note.duration) {
        float time_offset = t - note.time_offset;
        float note_hz = hz_from_semitone(note.semitone);
        float wave_val;

        switch (cur_notes[j].wave_function) {
            case WAVE_SAW:
                wave_val = wave_saw(time_offset, note_hz, note.tweak);
                break;
            case WAVE_TRI:
                wave_val = wave_tri(time_offset, note_hz, note.tweak);
                break;
            case WAVE_SQR:
                wave_val = wave_sqr(time_offset, note_hz, note.tweak);
                break;
            case WAVE_SIN:
            default:
                wave_val = wave_sin(time_offset, note_hz, note.tweak);
                break;
        }
        return note.volume * wave_val;
    } else {
        return 0.0f;
    }
}

// cleans up expired notes
void cleanup_notes() {
    int i;
    for (i = 0; i < MAX_NUM_NOTES; ++i) {
        if (notes_enabled[i]) {
            if ((float)total_frames_processed / SAMPLE_RATE > cur_notes[i].time_offset + cur_notes[i].duration) {
                remove_note(i);
            }
        }
    }
}



