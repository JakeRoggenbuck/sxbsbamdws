#include <stdio.h>
#include <stdlib.h>

const char *PATH = "/sys/class/backlight/amdgpu_bl0/brightness";

int write_brightness(int brightness) {
    if (brightness < 0 || brightness > 255) {
        return -1;
    }

    FILE *p = fopen(PATH, "w");
    fprintf(p, "%d", brightness);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        int brightness = atoi(argv[1]);
        write_brightness(brightness);
    } else if (argc > 2) {
        fprintf(stderr, "Too many args.\n");
    } else {
        fprintf(stderr, "Too few args.\n");
    }

    return 0;
}
