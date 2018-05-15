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
#include <iterator>

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

    if (player.townNumber == 19)
    {
        moveOn.draw(g,"Win",15);
    }
    else
    {
        moveOn.draw(g, "Head Out", 15);
    }
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
    return false;
}

void graphicsMain(Graphics& g)
{
    Item wheel = {"Wheel", 20,{1,0,0,0}, "A simple Wheel",0};
    Item axel = {"Axel", 21, {0,2,10,0}, "A wooden Axel used in carts and wagons",0};
    Item cheese = {"Cheese", 20, {0,1,0,0}, "Cheese!",1};
    vector<Item> standardShop = {wheel, axel, cheese};
    string generalStoreDescrip = "The general store is before you.";

    // 0 is beginning
    // 1,2 are the first branches
    // 3,4,5,6 are the second branches

    Town lymestone = {0, "Lymestone", "Trees gently sway as the mountains dwarf this small town to the east of Lancaster.|This is the town in which your adventure will begin.",
                      standardShop, generalStoreDescrip, {1,2}, -1, 2};
    Town sheffield = {1, "Sheffield", "As you emerge from a grove of trees, you see|Sheffield sprawling out in front of you", standardShop,
                      generalStoreDescrip, {0,2,4,5,6}, 1, 3};
    Town manchester = {2, "Manchester", "As you crest a grassy knoll,|the settlement of Manchester sprawls| out below you", standardShop,
                       "The general store is before you.", {0,1,3,4}, 1, 4};
    Town chester = {3, "Chester", "This village sits quietly.|You enjoyed the view of the sea you got|on your way here", standardShop,
                   generalStoreDescrip, {2,4}, 2, 4};
    Town penkhull = {4, "Penkhull", "Penkhull Descrip Here", standardShop, generalStoreDescrip, {1,2,3,5,7}, 2, 4};
    Town derby = {5, "Derby", "Derby Description Here", standardShop, generalStoreDescrip, {1,2,4,6,7}, 2, 5};
    Town nottingham = {6, "Nottingham", "Nottingham Description Here", standardShop, generalStoreDescrip, {1,5,7}, 2, 4};
    Town birmingham = {7, "Birmingham", "Birmingham Description Here", standardShop, generalStoreDescrip, {4,5,6,8,9,10,11,12}, 3, 5};
    Town leicester = {8, "Leicester", "Leicester Description Here", standardShop, generalStoreDescrip, {5,6,7,12,13}, 3, 4};
    Town hereford = {9, "Hereford", "Hereford Description Here", standardShop, generalStoreDescrip, {7,10}, 8, 5};
    Town gloucester = {10, "Gloucester", "Gloucester Description Here", standardShop, generalStoreDescrip, {7,9,14,15}, 5, 4};
    Town banbury = {11, "Banbury", "Banbury Description Here", standardShop, generalStoreDescrip, {7,10,12,16}, 2, 2};
    Town northhampton = {12, "Northhampton", "Northhampton Description Here", standardShop, generalStoreDescrip, {7,8,11,13,17}, 6, 4};
    Town peterborough = {13, "Peterborough", "Peterborough Description Here", standardShop, generalStoreDescrip, {8,12}, 1, 6};
    Town bristol = {14, "Bristol", "Bristol Description Here", standardShop, generalStoreDescrip, {10}, 9, 4};
    Town swindon = {15, "Swindon", "Swindon Description Here", standardShop, generalStoreDescrip, {10,14,16}, 5, 3};
    Town oxford = {16, "Oxford", "Oxford Description Here", standardShop, generalStoreDescrip, {11,15,17,18}, 7, 3};
    Town cambridge = {17, "Cambridge", "Cambridge Description Here", standardShop, generalStoreDescrip, {12,13,18}, 4,4};
    Town london = {18, "London", "London Description Here", standardShop, generalStoreDescrip, {16,17,19}, 6, 3};
    Town canterbury = {19, "Canterbury", "Canterbury Description Here", standardShop, generalStoreDescrip, {18}, 10, 5};

    vector<Town> towns = {lymestone, sheffield, manchester, chester, penkhull, derby, nottingham,
                          birmingham, leicester, hereford, gloucester, banbury, northhampton, peterborough,
                          bristol, swindon, oxford, cambridge, london, canterbury};

    Image landOne("C:\\Users\\Wyatt\\Desktop\\Lymestone Trail\\pretty.png");
    Image landTwo("C:\\Users\\Wyatt\\Desktop\\Lymestone Trail\\landTwo.png");

    Image walkOne("C:\\Users\\Wyatt\\Desktop\\Lymestone Trail\\walk1.png");
    Image walkTwo("C:\\Users\\Wyatt\\Desktop\\Lymestone Trail\\walk2.png");
    Image walkThree("C:\\Users\\Wyatt\\Desktop\\Lymestone Trail\\walk3.png");

    vector<Image> walks = {walkOne,walkTwo,walkOne,walkThree};

    PlayerData player = {4, {10,10,10,10}, {0,0,0,0}, walks};

    Image sprite = walks[0];

    vector<Image> landscapes = {landOne, landTwo};
    double initialWidth = 0;
    double initialHeight = 0;

    g.clear();

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
    int checkDangerIterator = 0;
    int foodIterator = 0;
    int foodThreshold = 150 - player.travelPace*10;
    int dangerThreshold = player.difficulty * 50;

    while (g.draw())
    {
        if (!travelling)
        {
            if(!town(g,player,towns[player.townNumber]))
            {
                return;
            }

            if (player.townNumber == 19)
            {
                player.endGame(g);
                break;
            }

            int newTownNum = player.selectDestination(g, towns);
            travelling = true;
            player.townNumber = newTownNum;
        }

        if (player.companionNames.size() == 0)
        {
            g.clear();
            g.text(200,200,50,"You Died, press Esc to exit",RED);
            while (g.draw())
            {
                for(const Event& e : g.events())
                {
                    if (e.evtType == EvtType::KeyPress)
                    {
                        if (e.arg == 16777216)
                        {
                            g.setCloseOnExit(true); //------------------------ FINISH
                            return;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < player.badnessLevel.size(); ++i)
        {
            player.badnessLevel[i] = player.diseaseState[i] + (player.travelPace - 5) +
                                      towns[player.townNumber].dangerLevel;
        }

        if(reloadHUD)
        {
            if (checkDangerIterator >= dangerThreshold)
            {
                player.checkForBadness(g);
                checkDangerIterator = 0;
            }
            else
            {
                checkDangerIterator += 1;
            }

            if (foodIterator >= foodThreshold)
            {
                player.checkFood(g);
                foodIterator = 0;
            }
            else
            {
                foodIterator += 1;
            }

            g.clear();
            g.image(20,0, land);
            g.image(playerX,playerY, sprite);
            drawHUD(g);
            player.printHealth(g);
            inventory.draw(g,"Inventory",15);
            changePace.draw(g,"Change Pace",15);
            changeRations.draw(g,"Ration Food/Water",15);
            stopToRest.draw(g,"Stop to Rest",15);
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
                    break;
                }
                if (changePace.isButtonPressed(e))
                {
                    velocity = player.changePace(g);
                    reloadHUD = true;
                    break;
                }
                if (changeRations.isButtonPressed(e))
                {

                    break;
                }
                if (stopToRest.isButtonPressed(e))
                {
                    player.stopToRest(g);
                    reloadHUD = true;
                    break;
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
                    sprite = player.updateSprite();
                }
                if (e.arg == (int)Key::Right && playerX <960)
                {
                    reloadHUD = true;
                    playerX = playerX + velocity;
                    sprite = player.updateSprite();
                }
                if (e.arg == (int)Key::Up && playerY > 0)
                {
                    reloadHUD = true;
                    playerY = playerY - velocity;
                    sprite = player.updateSprite();
                }
                if (e.arg == (int)Key::Down && playerY < 560)
                {
                    reloadHUD = true;
                    playerY = playerY + velocity;
                    sprite = player.updateSprite();
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

        if(player.checkForDeath(g))
        {
            reloadHUD = true;
        }
    }
}

int main()
{
    Graphics g("Graphics App", graphicsMain);
}
