#ifndef PALETTE_H
#define PALETTE_H

typedef struct RGBA {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} RGBA;

#define NUM_COLORS 8

#define COLOR_BLACK   (RGBA){0,0,0,255}
#define COLOR_RED     (RGBA){255,0,0,255}
#define COLOR_GREEN   (RGBA){0,255,0,255}
#define COLOR_YELLOW  (RGBA){255,255,0,255}
#define COLOR_BLUE    (RGBA){0,0,255,255}
#define COLOR_MAGENTA (RGBA){255,0,255,255}
#define COLOR_CYAN    (RGBA){0,255,255,255}
#define COLOR_WHITE   (RGBA){255,255,255,255}

#endif // PALETTE_H
