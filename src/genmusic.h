
// generates music

#ifndef GENMUSIC_H
#define GENMUSIC_H

#define NOTE_STR_LEN (10*8+20*10)


#include "noteman.h"
#include "tinymusicbox.h"

void str_from_note(char ** c, note_t note);

void note_from_str(note_t note, char ** c);

void update_notes();

#endif

