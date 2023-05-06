#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum VERB { INC, DEC, SET };

const char *PATH = "/sys/class/backlight/amdgpu_bl0/brightness";

void usage() {
    printf("Usage: sxbsbamdws [verb] [value]\n\nVerb options are inc, dec, "
           "set, and get.\n");
}
void version() { printf("sxbsbamdws: version 0.1.0\n"); }

int get_brightness() {
    char *line;
    size_t len = 0;
    int cur;
    FILE *rp = fopen(PATH, "r");
    if (rp == NULL) {
        fprintf(
            stderr,
            "Could not open file.\nMight be a permissions error, try sudo.\n");
        exit(EXIT_FAILURE);
    }

    getline(&line, &len, rp);

    cur = atoi(line);
    fclose(rp);
    return cur;
}

int write_brightness(enum VERB verb, int brightness) {
    if (verb != SET) {
        int cur = get_brightness();
        if (verb == INC) {
            brightness += cur;
        } else {
            brightness = cur - brightness;
        }
    }

    if (brightness > 255) {
        brightness = 255;
    } else if (brightness < 0) {
        brightness = 0;
    }

    FILE *p = fopen(PATH, "w");
    if (p == NULL) {
        fprintf(
            stderr,
            "Could not open file.\nMight be a permissions error, try sudo.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(p, "%d", brightness);
    fclose(p);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "get") == 0) {
            printf("%d\n", get_brightness());
            return 0;
        }
    }

    if (argc == 3) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            usage();
            return 0;
        } else if (strcmp(argv[1], "-v") == 0 ||
                   strcmp(argv[1], "--version") == 0) {
            version();
            return 0;
        }

        enum VERB verb;
        if (strcmp(argv[1], "inc") == 0) {
            verb = INC;
        } else if (strcmp(argv[1], "dec") == 0) {
            verb = DEC;
        } else if (strcmp(argv[1], "set") == 0) {
            verb = SET;
        } else {
            fprintf(stderr, "Verb not found. Try set, get, int, or dec.");
            return 0;
        }

        int brightness = atoi(argv[2]);
        write_brightness(verb, brightness);
    } else if (argc > 3) {
        fprintf(stderr, "Too many args.\n");
        usage();
    } else {
        fprintf(stderr, "Too few args.\n");
        usage();
    }

    return 0;
}
