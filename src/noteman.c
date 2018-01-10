
// note manager

#include "noteman.h"
#include "tinymusicbox.h"
#include "pa_player.h"
#include "waves.h"



// which notes are being played
note_t cur_notes[MAX_NUM_NOTES];
bool notes_enabled[MAX_NUM_NOTES];

// the default value (offset, duration, volume, tweak, wet, wave_function, semitone, A, D, S, R)
note_t default_note = { 0.0f, 4.0f, 1.0f, 0.0f, 0.0f, WAVE_SIN, 48 , 0.0f, 0.0f, 1.0f, 0.0f };


// note manipulation functions


note_t create_note(float offset, float dur, float vol, float tweak, float wetmix, int wave_function, int semitone, float A, float D, float S, float R) {
    note_t res = default_note;
    res.time_offset = offset;
    res.duration = dur;
    res.volume = vol;
    res.tweak = tweak;
    res.wet = wetmix;
    res.wave_function = wave_function;
    res.semitone = semitone;
    res.A = A;
    res.D = D;
    res.S = S;
    res.R = R;
    return res;
}

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
    if (t >= 0 && t <= note.duration + note.R) {
        float note_hz = hz_from_semitone(note.semitone);
        float wave_val;

        switch (note.wave_function) {
            case WAVE_SAW:
                wave_val = wave_saw(t, note_hz, note.tweak);
                break;
            case WAVE_TRI:
                wave_val = wave_tri(t, note_hz, note.tweak);
                break;
            case WAVE_SQR:
                wave_val = wave_sqr(t, note_hz, note.tweak);
                break;
            case WAVE_NOISE:
                wave_val = wave_noise(t, note_hz, note.tweak);
                break;
            case WAVE_SIN:
            default:
                wave_val = wave_sin(t, note_hz, note.tweak);
                break;
        }
        float env_fact = envelope_factor(note, t);
        printf("%f, ", env_fact);
        return note.volume * wave_val;
    } else {
        return 0.0f;
    }
}

// cleans up expired notes
void cleanup_notes(float t) {
    int i;
    for (i = 0; i < MAX_NUM_NOTES; ++i) {
        if (notes_enabled[i]) {
            if (t > cur_notes[i].time_offset + cur_notes[i].duration + cur_notes[i].R) {
                remove_note(i);
            }
        }
    }
}



