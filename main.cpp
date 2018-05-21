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
    g.polygon({{970,0},{(double)g.width(), 0.0},{(double)g.width(), 670.0},{970, 670}}, BLACK, BLACK);
    g.polygon({{10,670},{(double)g.width(),670.0},{(double)g.height(),(double)g.width()},{10.0, (double)g.height()}}, BLACK, BLACK);
    //g.polygon({{0,0},{20,0},{20,670},{0,670}},BLACK,BLACK);
    g.line(10,570,10,0, WHITE);
    g.line(970,570,970,0,WHITE);
    return;
}

void printText(Graphics& g, int x, int y, string text, Color textColor)
{
    vector<string> formattedText;
    size_t breakIndex = text.find("|");

    while ((int)breakIndex != -1)
    {
        formattedText.push_back(text.substr(0,breakIndex));
        text = text.substr(breakIndex+1, text.size()-1);
        breakIndex = text.find("|");
    }

    if ((int)formattedText.size() == 0 || (int)breakIndex == -1)
    {
        formattedText.push_back(text);
    }

    for (int p = 0; p < (int)formattedText.size(); ++p)
    {
        g.text({(double)x, (double)(y + p*16)}, 15, formattedText[p], textColor);
    }

    return;
}

bool town(Graphics& g, PlayerData& player, Town& town)
{
    int textYVal = 0;
    int velocity = 1;

    while (textYVal < 600)
    {
        for (const Event& e : g.events())
        {
            if (e.evtType == EvtType::KeyPress && e.arg == 70)
            {
                velocity = 10;
            }
            else
            {
                velocity = 1;
            }
        }
        g.clear();
        g.image(15,0,town.townPic);
        printText(g, 485, textYVal, town.townName, PURPLE);
        printText(g, 485, textYVal+30, town.townDesc, BLACK);

        drawHUD(g);
        textYVal = textYVal + velocity;
        g.draw(1);
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
                    g.image(20,0,town.townPic);
                    drawHUD(g);
                    moveOn.draw(g, "Head Out", 15);
                    shop.draw(g,"Shop",15);
                    talk.draw(g,"Talk",15);
                    inventory.draw(g,"Inventory",15);
                }
                if (talk.isButtonPressed(e))
                {
                    player.conversation(g);
                    drawHUD(g);
                    g.image(20,0,town.townPic);
                    moveOn.draw(g, "Head Out", 15);
                    shop.draw(g,"Shop",15);
                    talk.draw(g,"Talk",15);
                    inventory.draw(g,"Inventory",15);
                }
                if (inventory.isButtonPressed(e))
                {
                    if(!player.checkInventory(g))
                    {
                        return false;
                    }
                    g.clear();
                    drawHUD(g);
                    g.image(20,0,town.townPic);
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
    Item cheese = {"Cheese", 20, {0,0,1,0}, "A small wheel of cheese for you|and your friends to eat."};
    Item knight = {"Knight", 500, {10,0,0,0}, "A knight to acompany your party for protection."};
    Item horse = {"Horses", 40, {40,0,0,0}, " 4 horses to speed your company along."};
    vector<Item> standardShop = {cheese, knight,horse};
    string generalStoreDescrip = "The general store is before you.";

    Town lymestone = {0, "Lymestone", "Trees gently sway as the mountains|dwarf this small town to the east of Lancaster.|This is the town in which your adventure will begin.",
                      standardShop, generalStoreDescrip, {1,2}, -1, 2, Image("lymestone.png")};
    Town sheffield = {1, "Sheffield", "As you emerge from a grove of trees, you see|Sheffield sprawling out in front of you.|People walk around what looks to be|a weekly market.", standardShop,
                      generalStoreDescrip, {0,2,4,5,6}, 1, 3, Image("sheffield.png")};
    Town manchester = {2, "Manchester", "As you crest a grassy knoll,|the settlement of Manchester sprawls| out below you.||The town Reeve seems to be speaking|with a local farmer as you|walk into the town", standardShop,
                       "The general store is before you.", {0,1,3,4}, 1, 4, Image("manchester.png")};
    Town chester = {3, "Chester", "This village sits quietly.|You enjoyed the view of the sea you got|on your way here and, in fact, you can|now see the docks, bereft of|workers.", standardShop,
                   generalStoreDescrip, {2,4}, 2, 4, Image("chester.png")};
    Town penkhull = {4, "Penkhull", "The sprawling fields of this|farming town lay before you|as you come upon the main|buildings that make up|Penkhull",
                     standardShop, generalStoreDescrip, {1,2,3,5,7}, 2, 4, Image("penkhull.png")};
    Town derby = {5, "Derby", "As you cross the bridge into Derby,|you see the newly built Cathedral|sitting next to it.||People are coming in and out; you must have gotten here just|after matins",
                  standardShop, generalStoreDescrip, {1,2,4,6,7}, 2, 5, Image("derby.png")};
    Town nottingham = {6, "Nottingham", "Nottingham Castle sits above the other|buildings in this town.|You see people going about their business|throughout the streets.||You must have arrived at a busy time|of the day.",
                       standardShop, generalStoreDescrip, {1,5,7}, 2, 4,Image("nottingham.png")};
    Town birmingham = {7, "Birmingham", "The smell of smoke hits you as|you crest the hill between|you and the town of Birmingham.||As you look over the city, you|see several buildings are|charred and half destroyed;|they must be recovering from a fire.",
                       standardShop, generalStoreDescrip, {4,5,6,8,9,10,11,12}, 3, 5,Image("birmingham.png")};
    Town leicester = {8, "Leicester", "As you walk through this town,|you see some artisans working on|building a large gateway in|front of Leicester Castle.|Apparently, it is to be called|The Magazine.",
                      standardShop, generalStoreDescrip, {5,6,7,12,13}, 3, 4,Image("leicester.png")};
    Town hereford = {9, "Hereford", "As you all pass through this| town, you see people coming|in and out of a Cathedral|marked as a Grammar School.",
                     standardShop, generalStoreDescrip, {7,10}, 8, 5,Image("hereford.png")};
    Town gloucester = {10, "Gloucester", "As you passe throug Gloucester,|you starte to| see cartes and|whorses bring corne in an'|out of the toun.",
                       standardShop, generalStoreDescrip, {7,9,14,15}, 5, 4,Image("gloucester.png")};
    Town banbury = {11, "Banbury", "While passing throug the towne|of Banbury, you are astaunded|to see a surplus of chease for|sale in the marquet. In pardiculaar,|yeou see one weel thate|is 80lbs.",
                    standardShop, generalStoreDescrip, {7,10,12,16}, 2, 2,Image("banbury.png")};
    Town northampton = {12, "Northampton", "As you passe throug Northampton| you arre shoked to|witnes neumerus billdings in ruin and|disrapair. They must be|sufering economicaly.",
                         standardShop, generalStoreDescrip, {7,8,11,13,17}, 6, 4,Image("northampton.png")};
    Town peterborough = {13, "Peterborough", "You hafe to doge som|faling rocs as you passe by|the site of the construcshon|of St. John the Baptist.|The rest of the towne is|farely com thoug",
                         standardShop, generalStoreDescrip, {8,12}, 1, 6,Image("peterborough.png")};
    Town bristol = {14, "Bristol", "Won of the moste imbrezife|theengs youve sean ly|bephore you in the centre|ef Bristol. This is thee|High Cross ef Bristol",
                    standardShop, generalStoreDescrip, {10}, 9, 4,Image("bristol.png")};
    Town swindon = {15, "Swindon", "As oou pas throg te striets|of Swindon, oou here|wispers of the|lejend of the Dred Pirat|Yarmin Swindleson",
                    standardShop, generalStoreDescrip, {10,14,16}, 5, 3,Image("swindon.png")};
    Town oxford = {16, "Oxford", "Ae wyse robed scolere|pears at oou from the|windo of the Unifersetie|Colage Oxford",
                   standardShop, generalStoreDescrip, {11,15,17,18}, 7, 3,Image("oxford.png")};
    Town cambridge = {17, "Cambridge", "Fron the nunber ef|tapistrees an weevars,|thes towne seams to bee|kuite relient en thee|wool indestree.",
                      standardShop, generalStoreDescrip, {12,13,18}, 4,4,Image("cambridge.png")};
    Town london = {18, "London", "The Tower of London|kaches yur ey|befor anying alse en thee|towne of London. Yu arr|almest too Canterbury!",
                   standardShop, generalStoreDescrip, {16,17,19}, 6, 3,Image("london.png")};
    Town canterbury = {19, "Canterbury", "As the tomb of|Thomas Becket comes|into view, along with the|Cathedral at Canterbury, you|know that you have|finally made it. You have|completed your pilgramage!|||||But wait...|now you have to go back.||Uh Oh...",
                       standardShop, generalStoreDescrip, {18}, 10, 5,Image("canterbury.png")};

    vector<Town> towns = {lymestone, sheffield, manchester, chester, penkhull, derby, nottingham,
                          birmingham, leicester, hereford, gloucester, banbury, northampton, peterborough,
                          bristol, swindon, oxford, cambridge, london, canterbury};

    Image landOne("land1.png");
    Image landTwo("land2.png");
    Image landThree("land3.png");
    Image landFour("land4.png");
    Image landFive("land5.png");
    Image landSix("land6.png");


    Image walkOne("walk1.png");
    Image walkTwo("walk2.png");
    Image walkThree("walk3.png");

    vector<Image> walks = {walkOne,walkTwo,walkOne,walkThree};

    PlayerData player = {4, {10,10,10,10}, {0,0,0,0}, walks};

    Image sprite = walks[0];

    vector<Image> landscapes = {landOne, landTwo, landThree, landFour, landFive, landSix};
    double initialWidth = 0;
    double initialHeight = 0;

    g.clear();

    if (!(player.titleScreen(g, initialWidth, initialHeight)))
    {
        g.setCloseOnExit(true);
        return;
    }

    if (player.companionNames[0] == "outlaw")
    {
        player.difficulty = 0;
        player.money[0] = 0;
    }
    if (player.companionNames[1] == "yarmin")
    {
        player.money[0] = 500;
    }
    if (player.companionNames[2] == "mikeynducklings")
    {
        player.townNumber = 16;
    }
    if (player.companionNames[3] == "ght678i")
    {
        player.townNumber = 19;
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
    double foodThreshold = 150 - player.travelPace*10;
    int dangerThreshold = player.difficulty * 40;
    double foodMultiplier = 1;
    int chanceOfBrigands = 35;

    switch(player.difficulty)
    {
    case 3:
        chanceOfBrigands = 50;
        break;
    case 2:
        chanceOfBrigands = 10;
        break;
    case 1:
        chanceOfBrigands = 99;
        break;
    }

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
                g.setCloseOnExit(true);
                return;
            }

            int newTownNum = player.selectDestination(g, towns);
            travelling = true;
            player.townNumber = newTownNum;
            player.travelPace = 5;
            velocity = 5;
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

        for (int i = 0; i < (int)player.badnessLevel.size(); ++i)
        {
            player.badnessLevel[i] = (player.diseaseState[i] + (player.travelPace - 5) +
                                      towns[player.townNumber].dangerLevel + player.rations)/2;
        }

        if(reloadHUD)
        {
            player.cleanseInventory();
            foodThreshold = (150 - player.travelPace*10)*foodMultiplier;
            if (checkDangerIterator >= dangerThreshold) {
                player.checkForBadness(g);
                player.checkBrigands(g,(int)chanceOfBrigands);
                checkDangerIterator = 0; }
            else{ checkDangerIterator += 1; }

            if (foodIterator >= foodThreshold){player.checkFood(g); foodIterator = 0;}
            else { foodIterator += 1; }

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
                    foodMultiplier = player.changeRations(g);
                    reloadHUD = true;
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
            int i = g.randomInt(0,1);
            if (i == 1)
            {
                player.removeFromInventory("Horses", 1);
            }
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
