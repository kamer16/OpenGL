#include "options.hpp"
#include <getopt.h>
#include <stdio.h>

void
options::parse_args(int argc, char *argv[])
{
    while (1) {
        static struct option long_options[] = {
            {"resolution", required_argument, 0, 'r' },
            {"mesh",       required_argument, 0, 'm' },
            {"fullscreen",       no_argument, 0, 'm' },
            {0,            0,                 0,  0 }
        };

        int option_index;
        int c = getopt_long(argc, argv, "fm:r:x:", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
            case 'r':
            case 'x':
                sscanf(optarg, "%dx%d", &window_width, &window_height);
                break;
            case 'm':
                mesh_file = optarg;
                break;
            case 'f':
                fullscreen = 1;
                break;

            case '?':
                break;

            default:
                printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }
}



