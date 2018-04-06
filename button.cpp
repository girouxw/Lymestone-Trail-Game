#include "button.h"
#include "graphics.h"

using namespace mssm;
using namespace std;

Button::Button(Vec2d topL, Vec2d botR)
{
    topLeft = topL;
    bottomRight = botR;
}

void Button::draw(mssm::Graphics& g, std::string text, int size)
{
    vector<Vec2d> pts = {{topLeft.x, topLeft.y},
                         {bottomRight.x, topLeft.y},
                         {bottomRight.x, bottomRight.y},
                         {topLeft.x, bottomRight.y}};

    g.polygon(pts, WHITE, TRANSPARENT);
    g.text((topLeft.x + bottomRight.x)/2 - size*2.25, (topLeft.y + bottomRight.y)/2 + size/3, size, text, WHITE);
}

bool Button::isButtonPressed(const Event& e)
{
    return (e.x > topLeft.x && e.x < bottomRight.x) &&
           (e.y > topLeft.y && e.y < bottomRight.y);
}
