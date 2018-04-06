#define _USE_MATH_DEFINES

#include "graphics.h"
#include "playerdata.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include "button.h"
#include "item.h"

using namespace std;
using namespace mssm;

void graphicsMain(Graphics& g)
{
    PlayerData player = {0, {0,0,0,0}, {100,0,0,0}};

    double initialWidth = 0;
    double initialHeight = 0;

    if (!(player.titleScreen(g, initialWidth, initialHeight)))
    {
        g.setCloseOnExit(true);
        return;
    }

    while (g.draw())
    {

    }
}

int main()
{
    Graphics g("Graphics App", graphicsMain);
}
