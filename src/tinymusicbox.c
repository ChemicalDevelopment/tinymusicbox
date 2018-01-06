
// main file

#include "tinymusicbox.h"

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


    return 0;
}


