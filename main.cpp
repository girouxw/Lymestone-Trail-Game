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
    g.polygon({{10,570},{970,570},{970,670},{10,670}}, WHITE, BLACK);
    g.polygon({{970,0},{g.width(), 0},{g.width(), 670},{970, 670}}, BLACK, BLACK);
    g.polygon({{10,670},{g.width(),670},{g.height(),g.width()},{10, g.height()}}, BLACK, BLACK);
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

bool town(Graphics& g, PlayerData& player, Town& town)
{
    int textYVal = 570 ;
    int velocity = 20;

    while (textYVal > -50)
    {
        g.clear();

        printText(g, 15, textYVal, town.townName, PURPLE);
        printText(g, 15, textYVal+30, town.townDesc, WHITE);
        drawHUD(g);
        textYVal = textYVal - velocity;
        g.draw(10);
    }
    Button moveOn = {{30, 590},{150,650}};
    Button shop = {{170,590},{290,650}};
    Button talk = {{310, 590},{430,650}};
    Button inventory = {{450,590},{570,650}};

    moveOn.draw(g, "Head Out", 15);
    shop.draw(g,"Shop",15);
    talk.draw(g,"Talk",15);
    inventory.draw(g,"Inventory",15);

    while (g.draw())
    {
        for(const Event& e : g.events())
        {
            if (e.evtType == EvtType::KeyPress)
            {
                if (e.arg == 16777216)
                {
                    g.setCloseOnExit(true); //------------------------ FINISH
                    return false;
                }
            }
            if (e.evtType == EvtType::MousePress)
            {
                if (moveOn.isButtonPressed(e))
                {
                    g.clear();
                    return true;
                }
                if (shop.isButtonPressed(e))
                {
                    player.shop(g,town.shop,town.shopDesc);
                    g.clear();
                    drawHUD(g);
                    moveOn.draw(g, "Head Out", 15);
                    shop.draw(g,"Shop",15);
                    talk.draw(g,"Talk",15);
                    inventory.draw(g,"Inventory",15);
                }
                if (talk.isButtonPressed(e))
                {
                    //randomConversation(g); //DEFINE ----------------------------------------------------------------
                }
                if (inventory.isButtonPressed(e))
                {
                    if(!player.checkInventory(g))
                    {
                        return false;
                    }
                    g.clear();
                    drawHUD(g);
                    moveOn.draw(g, "Head Out", 15);
                    shop.draw(g,"Shop",15);
                    talk.draw(g,"Talk",15);
                    inventory.draw(g,"Inventory",15);

                }
            }
        }
    }
}

void selectDestination(Graphics& g, PlayerData& player, Town& town)
{

}

void graphicsMain(Graphics& g)
{
    PlayerData player = {0, {0,0,0,0}, {0,0,0,0}};
    vector<Item> standardShop = {{"Wheel", 20,{1,0,0,0}, "A simple Wheel"}, {"Axel", 21, {0,2,10,0}, "A wooden Axel used in carts and wagons"}};
    Town lymestone = {0, "Lymestone", "Trees gently sway as the mountains dwarf this small town to the east of Lancaster.|This is the town in which your adventure will begin.", standardShop, "The general store is before you.", {1,2}, g.randomInt(0,10), -1};
    Town sheffield = {1, "Sheffield", "Sheffield Description|Here", standardShop, "The general store is before you.", {0,2}, g.randomInt(0,10), 1};
    Town nottingham = {2, "Nottingham", "Nottingham Description| Here", standardShop, "The general store is before you.", {0,1}, g.randomInt(0,10),1};
    vector<Town> towns = {lymestone, sheffield, nottingham};

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
/*
    switch (player.pathNumber)
    {
    case 1:
        vector<int> townOrder = {0,1};
        break;
    }
*/

    Button inventory = {{30, 590},{150,650}};
    Button changePace = {{170,590},{290,650}};
    Button stopToRest = {{310, 590},{430,650}};
    Button changeRations = {{450,590},{620,650}};


    while (g.draw())
    {
        if (!travelling)
        {

            if(!town(g,player,towns[player.townNumber]))
            {
                return;
            }

            int newTownNum = player.selectDestination(g, towns);
            travelling = true;
            player.townNumber = newTownNum;
        }

        g.clear();
        drawHUD(g);
        inventory.draw(g,"Inventory",15);
        changePace.draw(g,"Change Pace",15);
        changeRations.draw(g,"Ration Food/Water",15);
        stopToRest.draw(g,"Stop to Rest",15);


    }
}

int main()
{
    Graphics g("Graphics App", graphicsMain);
}
