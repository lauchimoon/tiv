#ifndef PALETTE_H
#define PALETTE_H

typedef struct RGBA {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} RGBA;

#define NUM_COLORS 8

#define RESET_COLOR "\x1b[48;5;0m"

RGBA colors[NUM_COLORS] = {
    (RGBA){0,0,0,255}, (RGBA){255,0,0,255}, (RGBA){0,255,0,255}, (RGBA){255,255,0,255},
    (RGBA){0,0,255,255}, (RGBA){255,0,255,255}, (RGBA){0,255,255,255}, (RGBA){255,255,255,255},
};

const char *color_codes[NUM_COLORS] = {
    "\x1b[48;5;16m", "\x1b[48;5;196m", "\x1b[48;5;40m", "\x1b[48;5;220m",
    "\x1b[48;5;21m", "\x1b[48;5;200m", "\x1b[48;5;45m", "\x1b[48;5;255m",
};

#endif // PALETTE_H
