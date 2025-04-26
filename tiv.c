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

Image load_image(const char *);
RGBA get_pixel_color(Image, int, int);
RGBA get_most_similar_color(RGBA);
double distance(RGBA, RGBA);
int sqr_difference(int, int);
int min_index(double *, int);
const char *map_color_to_ansi(RGBA);
int colors_equal(RGBA, RGBA);

void printcolor(RGBA c) {
    printf("(%d,%d,%d,%d)\n", c.r, c.g, c.b, c.a);
}

int main(int argc, const char **argv)
{
    if (argc < 2) {
        printf("usage: %s <image-file>\n", PROGRAM_NAME);
        return 1;
    }

    const char *filename = argv[1];
    Image image = load_image(filename);

    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            RGBA color = get_pixel_color(image, x, y);
            //RGBA quantized_color = get_most_similar_color(color);
            printf("%s \x1b[48;5;0m", map_color_to_ansi(color));
        }
        printf("\n");
    }

    free(image.data);
    return 0;
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

RGBA get_most_similar_color(RGBA rgba)
{
    RGBA colors[NUM_COLORS] = {
        COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW,
        COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE,
    };
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
        sqr_difference(rgba1.b, rgba2.b));
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

const char *map_color_to_ansi(RGBA rgba)
{
    if (colors_equal(rgba, COLOR_BLACK)) return "\x1b[48;5;16m";
    else if (colors_equal(rgba, COLOR_RED)) return "\x1b[48;5;196m";
    else if (colors_equal(rgba, COLOR_GREEN)) return "\x1b[48;5;40m";
    else if (colors_equal(rgba, COLOR_YELLOW)) return "\x1b[48;5;220m";
    else if (colors_equal(rgba, COLOR_BLUE)) return "\x1b[48;5;21m";
    else if (colors_equal(rgba, COLOR_MAGENTA)) return "\x1b[48;5;200m";
    else if (colors_equal(rgba, COLOR_CYAN)) return "\x1b[48;5;45m";
    else if (colors_equal(rgba, COLOR_WHITE)) return "\x1b[48;5;255m";
    else return "?";
}

int colors_equal(RGBA rgba1, RGBA rgba2)
{
    return rgba1.r == rgba2.r &&
           rgba1.g == rgba2.g &&
           rgba1.b == rgba2.b &&
           rgba1.a == rgba2.a;
}
