#define _USE_MATH_DEFINES

#include "graphics.h"
#include "playerdata.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include "button.h"
#include "item.h"
#include "town.h"

using namespace std;
using namespace mssm;

void drawHUD(Graphics& g)
{
    g.polygon({{10,570},{970,570},{970,670},{10,670}}, WHITE, );
    g.line(10,570,10,0, WHITE);
    g.line(970,570,970,0,WHITE);
    return;
}

void printText(Graphics& g, int x, int y, string text, Color textColor)
{
    vector<string> formattedText;
    size_t breakIndex = text.find("|");

    while (breakIndex != -1)
    {
        formattedText.push_back(text.substr(0,breakIndex));
        text = text.substr(breakIndex+1, text.size()-1);
        breakIndex = text.find("|");
    }

    if (formattedText.size() == 0 || breakIndex == -1)
    {
        formattedText.push_back(text);
    }

    for (int p = 0; p < formattedText.size(); ++p)
    {
        g.text({x, y + p*16}, 15, formattedText[p], textColor);
    }

    return;
}

void graphicsMain(Graphics& g)
{
    PlayerData player = {0, {0,0,0,0}, {100,0,0,0}};
    vector<Item> standardShop = {{"Wheel", 20,{1,0,0,0}, "A simple Wheel"}, {"Axel", 21, {0,2,10,0}, "A wooden Axel used in carts and wagons"}};
    Town lymestone = {0, "Lymestone", "Trees gently sway as the mountains dwarf this small town to the east of Lancaster.|This is the town in which your adventure will begin.", standardShop, "The general store is before you."};
    Town sheffield = {1, "Sheffield", "Sheffield Description|Here", standardShop, "The general store is before you."};
    vector<Town> towns = {lymestone, sheffield};

    double initialWidth = 0;
    double initialHeight = 0;

    if (!(player.titleScreen(g, initialWidth, initialHeight)))
    {
        g.setCloseOnExit(true);
        return;
    }

    g.clear();
    drawHUD(g);
    bool travelling = false;

    switch (player.pathNumber)
    {
    case 1:
        vector<int> townOrder = {0,1};
        break;
    }

    while (g.draw())
    {
        if (!travelling)
        {
            int textYVal = g.height();
            int velocity = 1;

            while (textYVal > 0)
            {
                g.clear();

                printText(g, 15, textYVal, towns[player.townNumber].townName, PURPLE);
                printText(g, 15, textYVal+30, towns[player.townNumber].townDesc, WHITE);
                drawHUD(g);
                textYVal = textYVal - velocity;
                g.draw(10);

            }

        }
    }
}

int main()
{
    Graphics g("Graphics App", graphicsMain);
}
