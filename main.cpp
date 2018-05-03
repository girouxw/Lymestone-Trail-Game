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
    //g.polygon({{0,0},{20,0},{20,670},{0,670}},BLACK,BLACK);
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

void graphicsMain(Graphics& g)
{
    PlayerData player = {0, {10,10,10,10}, {0,0,0,0}};
    vector<Item> standardShop = {{"Wheel", 20,{1,0,0,0}, "A simple Wheel"},
                                 {"Axel", 21, {0,2,10,0}, "A wooden Axel used in carts and wagons"}};

    Town lymestone = {0, "Lymestone", "Trees gently sway as the mountains dwarf this small town to the east of Lancaster.|This is the town in which your adventure will begin.",
                      standardShop, "The general store is before you.", {1,2}, -1, 2};
    Town sheffield = {1, "Sheffield", "Sheffield Description|Here", standardShop,
                      "The general store is before you.", {0,2}, 1, 3};
    Town nottingham = {2, "Nottingham", "Nottingham Description| Here", standardShop,
                       "The general store is before you.", {0,1}, 1, 4};

    vector<Town> towns = {lymestone, sheffield, nottingham};
    Image boy("C:\\Users\\Wyatt\\Desktop\\Game\\Lymestone-Trail-Game\\BetterestBoy.png");
    Image landOne("C:\\Users\\Wyatt\\Desktop\\Game\\Lymestone-Trail-Game\\pretty.png");
    Image landTwo("C:\\Users\\Wyatt\\Desktop\\Game\\Lymestone-Trail-Game\\landTwo.png");

    vector<Image> landscapes = {landOne, landTwo};
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


    Button inventory = {{30, 590},{150,650}};
    Button changePace = {{170,590},{290,650}};
    Button stopToRest = {{310, 590},{430,650}};
    Button changeRations = {{450,590},{620,650}};

    bool reloadHUD = true;
    int playerX = 894;
    int playerY = 500;
    int velocity = 5;

    int screenNumber = 1;
    Image land = landOne;


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

        for (int i = 0; i< player.badnessLevel.size(); ++i)
        {
            player.badnessLevel[i] = 10*player.difficulty*(player.diseaseState[i] + player.travelPace + towns[player.townNumber].dangerLevel);
        }

        if(reloadHUD)
        {
            g.clear();
            player.checkForBadness(g);
            g.image(20,0, land);
            g.image(playerX,playerY, boy);
            drawHUD(g);
            player.printHealth(g);
            inventory.draw(g,"Inventory",15);
            changePace.draw(g,"Change Pace",15);
            changeRations.draw(g,"Ration Food/Water",15);
            stopToRest.draw(g,"Stop to Rest",15);
            g.text(1000,50,20,to_string(player.badnessLevel[0]),PURPLE);

            reloadHUD = false;
        }

        for (const Event& e : g.events())
        {
            if (e.evtType == EvtType::MousePress)
            {
                if (inventory.isButtonPressed(e))
                {
                    player.checkInventory(g);
                    reloadHUD = true;
                }
                if (changePace.isButtonPressed(e))
                {
                    velocity = player.changePace(g);
                    reloadHUD = true;
                }
                if (changeRations.isButtonPressed(e))
                {

                }
                if (stopToRest.isButtonPressed(e))
                {
                    player.stopToRest(g);
                    reloadHUD = true;
                }
            }
            if (e.evtType == EvtType::KeyPress)
            {
                if (e.arg == 16777216)
                {
                    g.setCloseOnExit(true);
                    return;
                }
                if (e.arg == (int)Key::Left)
                {
                    reloadHUD = true;
                    playerX = playerX - velocity;
                }
                if (e.arg == (int)Key::Right && playerX <960)
                {
                    reloadHUD = true;
                    playerX = playerX + velocity;
                }
                if (e.arg == (int)Key::Up && playerY > 0)
                {
                    reloadHUD = true;
                    playerY = playerY - velocity;
                }
                if (e.arg == (int)Key::Down && playerY < 560)
                {
                    reloadHUD = true;
                    playerY = playerY + velocity;
                }
            }
        }
        if (playerX < 20 && screenNumber == towns[player.townNumber].numOfScreens)
        {
            playerX = 894;
            screenNumber = 1;
            travelling = false;
        }
        else if (playerX < 20 && screenNumber < towns[player.townNumber].numOfScreens)
        {
            playerX = 894;
            int i = g.randomInt(0,landscapes.size()-1);
            land = landscapes[i];
            screenNumber += 1;
        }





    }
}

int main()
{
    Graphics g("Graphics App", graphicsMain);
}
