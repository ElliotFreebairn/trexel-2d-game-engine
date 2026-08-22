#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

typedef struct
{
    int x;
    int y;
} Position;

typedef struct
{
    int width;
    int height;
} Size;

typedef struct
{
    int8_t r;
    int8_t g;
    int8_t b;
    int8_t a;
} Colour_RGBA8888;
/*
 * Improve this later
 */
typedef enum COLOUR
{
    RED,
    BLUE,
    GREEN,
    YELLOW,
    BROWN,
    WHITE,
    BLACK
} COLOUR;




#endif
