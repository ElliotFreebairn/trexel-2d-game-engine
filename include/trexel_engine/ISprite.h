#pragma once

class ISprite
{
private:
    virtual void setupSprite() = 0;
    virtual void resolveSpriteState() = 0;
};
