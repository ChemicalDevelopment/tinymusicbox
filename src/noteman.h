
// note manager

#ifndef NOTEMAN_H
#define NOTEMAN_H

// how many notes maximum being queued at one time
#define MAX_NUM_NOTES (256)

#define WAVE_SIN 0x11
#define WAVE_SQR 0x12
#define WAVE_TRI 0x13
#define WAVE_SAW 0x14


typedef struct note_t {

    // total offset since portaudio init (in seconds)
    float time_offset;

    // duration (in seconds) the note is to be played for
    float duration;

    // volume (multiplier) the note should be
    float volume;

    // tweak parameter that can change the waveform
    float tweak;

    // which wavefunction (see WAVE_* macros)
    int wave_function;

    // semitones from A0 (A4,440hz is semitone=48, A3,220hz is semitone=36). 12 for each octaves
    int semitone;

} note_t;


// which notes are being played
note_t cur_notes[MAX_NUM_NOTES];
bool notes_enabled[MAX_NUM_NOTES];

// the default value
note_t default_note;


// note manipulation functions

// adds a note to active notes
void add_note(note_t note);

// sets a note to inactive
void remove_note(int note_id);

// utility function
float hz_from_semitone(int semitone);

// returns the sample of the note at time 't'
float eval_note(note_t note, float t);

// cleans up expired notes
void cleanup_notes();


#endif

