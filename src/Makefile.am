
# what flags you want to pass to the C compiler & linker
AM_CFLAGS = --pedantic -Wall -std=c99 -O2
AM_LDFLAGS =


bin_PROGRAMS = tinymusicbox

tinymusicbox_SOURCES = tinymusicbox.h waves.h noteman.h pa_player.h gvst.h tinymusicbox.c waves.c noteman.c pa_player.c gvst.c
tinymusicbox_LDADD = -lportaudio
