#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include "graphics.h"
#include "vec2d.h"

class Button
{    
public:
    Vec2d topLeft;
    Vec2d bottomRight;

    Button(Vec2d topL,Vec2d botR);

    void draw(mssm::Graphics& g, std::string text, int size, mssm::Color color);
    void draw(mssm::Graphics& g, std::string text, int size);
    bool isButtonPressed(const mssm::Event& e);

};

#endif // BUTTON_H
