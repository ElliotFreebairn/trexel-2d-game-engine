#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL_render.h>

class Sprite
{
public:
    Sprite(int frameWidth, int frameHeight, int columns, int rows, float frameDuration = 1.0f);

    void update(float deltaTime);
    SDL_Rect getSrcRect() const;

    void setFrame(int frame) { currentFrame = frame % (columns * rows); }

private:
    int frameWidth, frameHeight;
    int columns, rows;
    int currentFrame = 0;
    float frameDuration;
    float elapsed = 0.0f;
};
#endif
