
// main file

#include "tinymusicbox.h"

#include "pa_player.h"
#include "noteman.h"
#include "genmusic.h"


#include <getopt.h>


int main(int argc, char ** argv) {

    char c;

    while ((c = getopt (argc, argv, "h")) != (char)-1) {
        switch (c) {
            case 'h':
                printf("tinymusicbox\n");
                printf("  -h       display this message\n");
                printf("\n");
                printf("Please email with any questions: <cade@cade.site>\n");
                return 0;
                break;
            case '?':
                printf("Unknown argument: '-%c', use '-h' to print usage\n", optopt);
                return 1;
            default:
                printf("Unkown getopt return val\n");
                return 1;
        }
    }


    srand(time(NULL));

    update_notes();

    tmb_pa_init();
    

    /*
    note_t cnote = snare_note;

    add_note(cnote);
    cnote.time_offset = 2.0f;
    add_note(cnote);

    */

    while (1);

    tmb_pa_stop();


    return 0;
}


