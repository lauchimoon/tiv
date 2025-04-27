#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "palette.h"
#ifdef __TINYC__
    #define STBI_NO_SIMD
#endif
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define PROGRAM_NAME "tiv"

typedef struct Image {
    unsigned char *data;
    int width;
    int height;
} Image;

int is_png(const char *);
Image load_image(const char *);
RGBA get_pixel_color(Image, int, int);
void display(RGBA);
RGBA get_most_similar_color(RGBA);
double distance(RGBA, RGBA);
int sqr_difference(int, int);
int min_index(double *, int);
char *map_color_to_ansi(RGBA);
int find_equal_color_index(RGBA);
int colors_equal(RGBA, RGBA);

int main(int argc, const char **argv)
{
    if (argc < 2) {
        printf("usage: %s <png-file>\n", PROGRAM_NAME);
        return 1;
    }

    const char *filename = argv[1];
    if (!is_png(filename)) {
        printf("error: '%s' is not a PNG file.\n", filename);
        return 2;
    }

    Image image = load_image(filename);

    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            RGBA color = get_pixel_color(image, x, y);
            RGBA quantized_color = get_most_similar_color(color);
            display(quantized_color);
        }
        printf("\n");
    }

    free(image.data);
    return 0;
}

int is_png(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
        return 1;

    char signature[4] = { 0 };
    fread(signature, sizeof(char), 4, f);
    fclose(f);

    return signature[0] == 0xffffff89 && signature[1] == 0x50 &&
        signature[2] == 0x4e && signature[3] == 0x47;
}

Image load_image(const char *filename)
{
    Image image = { 0 };

    image.data = stbi_load(filename, &image.width, &image.height, NULL, 0);

    return image;
}

RGBA get_pixel_color(Image image, int x, int y)
{
    RGBA rgba = { 0 };
    int index = 4*(y*image.width + x);

    rgba.r = image.data[index];
    rgba.g = image.data[index + 1];
    rgba.b = image.data[index + 2];
    rgba.a = image.data[index + 3];

    return rgba;
}

void display(RGBA rgba)
{
    char *ansi_code = map_color_to_ansi(rgba);
    printf("%s %s", ansi_code, RESET_COLOR);
    free(ansi_code);
}

RGBA get_most_similar_color(RGBA rgba)
{
    if (rgba.a == 0)
         // same color as terminal: emulate fully transparent pixel
        return colors[0];

    double color_differences[NUM_COLORS] = { 0.0 };

    for (int i = 0; i < NUM_COLORS; ++i) {
        RGBA color = colors[i];
        color_differences[i] = distance(rgba, color);
    }

    int most_similar_index = min_index(color_differences, NUM_COLORS);
    return colors[most_similar_index];
}

double distance(RGBA rgba1, RGBA rgba2)
{
    return sqrt(
        sqr_difference(rgba1.r, rgba2.r) +
        sqr_difference(rgba1.g, rgba2.g) +
        sqr_difference(rgba1.b, rgba2.b) +
        sqr_difference(rgba1.a, rgba2.a));
}

int sqr_difference(int x, int y)
{
    int diff = x - y;
    return diff*diff;
}

int min_index(double *array, int array_len)
{
    double min = 500.0;
    int index = 0;
    for (int i = 0; i < array_len; ++i) {
        if (min > array[i]) {
            min = array[i];
            index = i;
        }
    }

    return index;
}

char *map_color_to_ansi(RGBA rgba)
{
    int index = find_equal_color_index(rgba);
    if (index == -1)
        return "?";

#define ANSI_CODE_SIZE 256
    char *ansi_code = malloc(ANSI_CODE_SIZE + 1);
    sprintf(ansi_code, "\x1b[48;5;%dm", index);
    return ansi_code;
}

int find_equal_color_index(RGBA rgba)
{
    for (int i = 0; i < NUM_COLORS; ++i)
        if (colors_equal(rgba, colors[i]))
            return i;

    return -1;
}

int colors_equal(RGBA rgba1, RGBA rgba2)
{
    return rgba1.r == rgba2.r &&
           rgba1.g == rgba2.g &&
           rgba1.b == rgba2.b &&
           rgba1.a == rgba2.a;
}
