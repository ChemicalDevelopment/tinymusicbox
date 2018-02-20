
// note manager

#ifndef NOTEMAN_H
#define NOTEMAN_H

#include "stdbool.h"

// how many notes maximum being queued at one time
#define MAX_NUM_NOTES (256*4)

#define WAVE_SIN 0x11
#define WAVE_SQR 0x12
#define WAVE_TRI 0x13
#define WAVE_SAW 0x14
#define WAVE_NOISE 0x15
#define WAVE_808 0x16


typedef struct note_t {

    // total offset since portaudio init (in seconds)
    float time_offset;

    // duration (in seconds) the note is to be played for
    float duration;

    // volume (multiplier) the note should be
    float volume;

    // tweak parameter that can change the waveform
    float tweak;

    // wet mixer, 0.0 is dry, 1.0 is completely wet
    float wet;

    // which wavefunction (see WAVE_* macros)
    int wave_function;

    // semitones from A0 (A4,440hz is semitone=48, A3,220hz is semitone=36). 12 for each octaves
    int semitone;

    // envelope settings
    float A, D, S, R;

} note_t;



int num_notes_enabled;
// which notes are being played
note_t cur_notes[MAX_NUM_NOTES];
bool notes_enabled[MAX_NUM_NOTES];

// the default value
note_t default_note;

note_t snare_note;

note_t hat_note;


// note manipulation functions


// creates a note
note_t create_note(float offset, float dur, float vol, float tweak, float wetmix, int wave_function, int semitone, float A, float D, float S, float R);

// adds a note to active notes
void add_note(note_t note);

// sets a note to inactive
void remove_note(int note_id);

// utility function
float hz_from_semitone(int semitone);

// returns the sample of the note at time 't'
float eval_note(note_t note, float t);

// cleans up expired notes
void cleanup_notes(float t);


#endif

