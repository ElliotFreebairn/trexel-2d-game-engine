#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

typedef struct
{
    int8_t r;
    int8_t g;
    int8_t b;
    int8_t a;
} RGBA8888;

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

struct Rect
{
    int x, y, w, h;
};

enum class Flip
{
    NONE,
    HORIZONTAL,
    VERTICAL
};

#endif
