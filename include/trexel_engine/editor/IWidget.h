#pragma once

class IWidget
{
public:
    ~IWidget() = default;

    virtual void draw() = 0;
};
