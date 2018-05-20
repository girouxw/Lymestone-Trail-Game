#include "playerdata.h"
#include <vector>
#include <string>
#include "graphics.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include "button.h"
#include "item.h"

using namespace std;
using namespace mssm;


PlayerData::PlayerData(double numberOfCompanions, std::vector<int> startingHealth, std::vector<int> startingMoney, std::vector<Image> walks)
{
    numOfCompanions = numberOfCompanions;
    companionHealth = startingHealth;
    money = startingMoney;
    companionNames = {" "," "," "," "};
    inventory = {};
    difficulty = -1;
    diseaseState = {0,0,0,0};
    townNumber = 0;
    travelPace = 5;
    badnessLevel = {0,0,0,0};
    walkingSprites = walks;
    spriteNumber = 0;
    spriteIterator = 0;
    rations = 3;
    protection = 0;
}

bool PlayerData::titleScreen(mssm::Graphics& g, double& initialWidth, double& initialHeight)
{
    double width = g.width();
    double height = g.height();
    string stringWidth = to_string(width);
    string stringHeight = to_string(height);

    g.text(1300,100,10,stringWidth, WHITE);
    g.text(1300,115,10,stringHeight, WHITE);

    if (width != initialWidth || height != initialHeight)
    {
        g.clear();
        initialHeight = height;
        initialWidth = width;
        g.setBackground(BLACK);
        g.draw();
        g.text(20, 100, 100, "Lymestone Trail", PURPLE);
        g.line({970,0},{970,650}, WHITE);
        g.line({0,650},{970,650}, WHITE);

        Button startButton = {{20,130},{185,165}};
        Button exitButton = {{200,130},{365,165}};

        exitButton.draw(g, "Exit Game", 15);
        startButton.draw(g, "Start Game", 15);

        int p = 0;

        while (g.draw())
        {
            for(const Event& e : g.events())
            {
                if (e.evtType == EvtType::MousePress)
                {
                    if (exitButton.isButtonPressed(e))
                    {
                        return false;
                    }
                    if (startButton.isButtonPressed(e))
                    {
                        cout << "Selecting";
                        selectDifficulty(g);
                        selectCharacters(g);
                        //selectPath(g);
                        p = 1;
                    }
                }
            }
            if (p == 1)
            {
                break;
            }
        }
        while (g.draw())
        {
            //shop(g, startingShop,startingShopDesc);
            int i = readyToStart(g);

            switch(i)
            {
            case 1:
                return true;
            case 3:
                companionNames[0] = "";
                companionNames[1] = "";
                companionNames[2] = "";
                companionNames[3] = "";
                selectCharacters(g);
                break;
            }
        }

    }

    return 0;
}

int PlayerData::selectDestination(mssm::Graphics& g, vector<Town> allTowns)
{
    bool goodInput = false;
    while(!goodInput)
    {
        g.clear();
        drawHUDs(g);
        g.text(20, 40, 40, "From here, you can go to:", PURPLE);

        string possibilities = "";

        for(int i = 0; i< allTowns[townNumber].canGoTo.size(); ++i)
        {
            //int newTownNumber = allTowns[townNumber].canGoTo[i];

            possibilities.append(to_string(i+1));
            possibilities.append(".  ");
            possibilities.append(allTowns[allTowns[townNumber].canGoTo[i]].townName);
            possibilities.append("|");
        }

        printTextTwo(g, 40, 60, possibilities, WHITE);
        g.text(20,590,20, "Pick a Town Number:", WHITE);
        int choice = getNumber(g,220,590,20);
        if (choice > allTowns[townNumber].canGoTo.size() || choice == 0)
        {
            g.text(20,620,20,"Not an Option", RED);
            g.draw(1000);
        }
        else
        {
            int newTownNumber = allTowns[townNumber].canGoTo[choice - 1];
            return newTownNumber;
        }

    }
}

void PlayerData::selectPath(mssm::Graphics& g)
{
    g.clear();
    g.text(20,80,50, "Please Select a Path to Take", PURPLE);
    Button pathOne = {{20,130},{185,165}};

    pathOne.draw(g, "Path 1", 15);

    while(g.draw())
    {
        for(const Event& e : g.events())
        {
            if (e.evtType == EvtType::MousePress)
            {
                if (pathOne.isButtonPressed(e))
                {

                    return;
                }
                else
                {

                }
            }
        }
    }
}

void PlayerData::selectDifficulty(mssm::Graphics& g)
{
    g.clear();
    g.text(20, 80, 50, "Please Select a Difficulty Level", PURPLE);
    Button kingsHighwayDiff = {{20,130},{185,165}};
    Button aristocraftDiff = {{200,130},{365,165}};
    Button villaniDiff = {{20,175},{185,210}};
    Button poorPeasantDiff = {{200,175},{365,210}};

    kingsHighwayDiff.draw(g, "King's Highway", 15);
    aristocraftDiff.draw(g, "Aristocracy", 15);
    villaniDiff.draw(g, "Villani", 15);
    poorPeasantDiff.draw(g, "Peasant", 15);

    while (g.draw())
    {
        for(const Event& e : g.events())
        {
            if (e.evtType == EvtType::MousePress)
            {
                if (kingsHighwayDiff.isButtonPressed(e))
                {
                    difficulty = 3;
                    money = {100,0,0,0};
                    return;
                }
                if (aristocraftDiff.isButtonPressed(e))
                {
                    difficulty = 3;
                    money = {50,0,0,0};
                    return;
                }
                if (villaniDiff.isButtonPressed(e))
                {
                    difficulty = 2;
                    money = {10,0,0,0};
                    return;
                }
                if (poorPeasantDiff.isButtonPressed(e))
                {
                    difficulty = 2;
                    money = {1,0,0,0};
                    return;
                }
            }
        }
    }
}


void PlayerData::selectCharacters(mssm::Graphics& g)
{
    g.clear();
    g.text(15, 18, 18, "You and four friends are travelling to the village of Lymestone", PURPLE);
    g.text(15,34,18,"What are your names?", PURPLE);

    g.text(15, 55, 18, "Pilgrim 1:", WHITE);
    g.text(15, 71, 18, "Pilgrim 2:", WHITE);
    g.text(15, 87, 18, "Pilgrim 3:", WHITE);
    g.text(15, 103, 18, "Pilgrim 4:", WHITE);
    companionNames[0] = getText(g, 95, 55, 18);
    companionNames[1] = getText(g, 95, 71, 18);
    companionNames[2] = getText(g, 95, 87, 18);
    companionNames[3] = getText(g, 95, 103, 18);
    g.clear();
/*
    if (companionNames[0] = "outlaw")
    {
        difficulty = 8;
        money = {1,0,0,0};
    }
*/
}

string getText(Graphics& g, double x, double y, double size)
{
    string name = "";
    while (g.draw())
    {    
        for(const Event& e : g.events())
        {
            if (e.evtType == EvtType::KeyPress)
            {
                if (e.arg == 16777220 && name.size()>0) //Enter
                {
                    return name;
                }

                if (e.arg == 65) //a
                {
                    g.text(x+(name.size() * size *.667), y, size, "a", WHITE);
                    name.push_back('a');
                }
                else if (e.arg == 66) // b
                {
                    g.text(x+(name.size() * size *.667), y, size, "b", WHITE);
                    name.push_back('b');
                }
                else if (e.arg == 67) // etc...
                {
                    g.text(x+(name.size() * size *.667), y, size, "c", WHITE);
                    name.push_back('c');
                }
                else if (e.arg == 68)
                {
                    g.text(x+(name.size() * size *.667), y, size, "d", WHITE);
                    name.push_back('d');
                }
                else if (e.arg == 69)
                {
                    g.text(x+(name.size() * size *.667), y, size, "e", WHITE);
                    name.push_back('e');
                }
                else if (e.arg == 70)
                {
                    g.text(x+(name.size() * size *.667), y, size, "f", WHITE);
                    name.push_back('f');
                }
                else if (e.arg == 71)
                {
                    g.text(x+(name.size() * size *.667), y, size, "g", WHITE);
                    name.push_back('g');
                }
                else if (e.arg == 72)
                {
                    g.text(x+(name.size() * size *.667), y, size, "h", WHITE);
                    name.push_back('h');
                }
                else if (e.arg == 73)
                {
                    g.text(x+(name.size() * size *.667), y, size, "i", WHITE);
                    name.push_back('i');
                }
                else if (e.arg == 74)
                {
                    g.text(x+(name.size() * size *.667), y, size, "j", WHITE);
                    name.push_back('j');
                }
                else if (e.arg == 75)
                {
                    g.text(x+(name.size() * size *.667), y, size, "k", WHITE);
                    name.push_back('k');
                }
                else if (e.arg == 76)
                {
                    g.text(x+(name.size() * size *.667), y, size, "l", WHITE);
                    name.push_back('l');
                }
                else if (e.arg == 77)
                {
                    g.text(x+(name.size() * size *.667), y, size, "m", WHITE);
                    name.push_back('m');
                }
                else if (e.arg == 78)
                {
                    g.text(x+(name.size() * size *.667), y, size, "n", WHITE);
                    name.push_back('n');
                }
                else if (e.arg == 79)
                {
                    g.text(x+(name.size() * size *.667), y, size, "o", WHITE);
                    name.push_back('o');
                }
                else if (e.arg == 80)
                {
                    g.text(x+(name.size() * size *.667), y, size, "p", WHITE);
                    name.push_back('p');
                }
                else if (e.arg == 81)
                {
                    g.text(x+(name.size() * size *.667), y, size, "q", WHITE);
                    name.push_back('q');
                }
                else if (e.arg == 82)
                {
                    g.text(x+(name.size() * size *.667), y, size, "r", WHITE);
                    name.push_back('r');
                }
                else if (e.arg == 83)
                {
                    g.text(x+(name.size() * size *.667), y, size, "s", WHITE);
                    name.push_back('s');
                }
                else if (e.arg == 84)
                {
                    g.text(x+(name.size() * size *.667), y, size, "t", WHITE);
                    name.push_back('t');
                }
                else if (e.arg == 85)
                {
                    g.text(x+(name.size() * size *.667), y, size, "u", WHITE);
                    name.push_back('u');
                }
                else if (e.arg == 86)
                {
                    g.text(x+(name.size() * size *.667), y, size, "v", WHITE);
                    name.push_back('v');
                }
                else if (e.arg == 87)
                {
                    g.text(x+(name.size() * size *.667), y, size, "w", WHITE);
                    name.push_back('w');
                }
                else if (e.arg == 88)
                {
                    g.text(x+(name.size() * size *.667), y, size, "x", WHITE);
                    name.push_back('x');
                }
                else if (e.arg == 89)
                {
                    g.text(x+(name.size() * size *.667), y, size, "y", WHITE);
                    name.push_back('y');
                }
                else if (e.arg == 90)
                {
                    g.text(x+(name.size() * size *.667), y, size, "z", WHITE);
                    name.push_back('z');
                }
                else if (e.arg == 48)
                {
                    g.text(x+(name.size() * size *.667), y, size, "0", WHITE);
                    name.push_back('0');
                }
                else if (e.arg == 49)
                {
                    g.text(x+(name.size() * size *.667), y, size, "1", WHITE);
                    name.push_back('1');
                }
                else if (e.arg == 50)
                {
                    g.text(x+(name.size() * size *.667), y, size, "2", WHITE);
                    name.push_back('2');
                }
                else if (e.arg == 51)
                {
                    g.text(x+(name.size() * size *.667), y, size, "3", WHITE);
                    name.push_back('3');
                }
                else if (e.arg == 52)
                {
                    g.text(x+(name.size() * size *.667), y, size, "4", WHITE);
                    name.push_back('4');
                }
                else if (e.arg == 53)
                {
                    g.text(x+(name.size() * size *.667), y, size, "5", WHITE);
                    name.push_back('5');
                }
                else if (e.arg == 54)
                {
                    g.text(x+(name.size() * size *.667), y, size, "6", WHITE);
                    name.push_back('6');
                }
                else if (e.arg == 55)
                {
                    g.text(x+(name.size() * size *.667), y, size, "7", WHITE);
                    name.push_back('7');
                }
                else if (e.arg == 56)
                {
                    g.text(x+(name.size() * size *.667), y, size, "8", WHITE);
                    name.push_back('8');
                }
                else if (e.arg == 57)
                {
                    g.text(x+(name.size() * size *.667), y, size, "9", WHITE);
                    name.push_back('9');
                }
                else
                {

                }
            }
        }
    }

    return 0;
}

void PlayerData::printHealth(mssm::Graphics& g)
{
    string health;
    
    for(int i = 0; i < companionHealth.size(); ++i)
    {
        health.append(companionNames[i]);
        health.append(": ");
        health.append(to_string(companionHealth[i]));
        health.append("|");
    }

    printTextTwo(g, 20,20,health,PURPLE);
}

void PlayerData::stopToRest(mssm::Graphics& g)
{
    bool alreadyFKD = false;

    g.polygon({{295,100},{695,100},{695,300},{295,300}},WHITE,BLACK);
    g.text(300,200,20,"How many days do you rest?",WHITE);
    int daysToRest = getNumber(g,570,200,20);

    for (int i = 0; i < daysToRest; ++i)
    {
        int rand = g.randomInt(0,10);
        int randPlayer = g.randomInt(0,numOfCompanions-1);
        if(companionHealth[randPlayer] < 10)
        {
            companionHealth[randPlayer] += 1;
        }
        else if (rand == 0 && alreadyFKD)
        {
            companionHealth[g.randomInt(0,3)] -= 12;
            alreadyFKD = true;
        }
    }
}

Image PlayerData::updateSprite()
{
    if (spriteIterator < 2)
    {
        spriteIterator++;
        return walkingSprites[spriteNumber];
    }

    spriteIterator = 0;

    if (spriteNumber > 2)
    {
        spriteNumber = 0;
    }
    else
    {
        spriteNumber += 1;
    }

    return walkingSprites[spriteNumber];
}

bool PlayerData::checkForDeath(mssm::Graphics& g)
{
    for (auto i = 0; i < companionHealth.size(); ++i)
    {
        if (companionHealth[i] <= 0)
        {
            Button back = {{600,120},{690,200}};
            string deadName = companionNames[i];
            deadName.append(" has died.");
            g.polygon({{295,100},{695,100},{695,300},{295,300}},WHITE,BLACK);
            g.text(300,200,20,deadName,WHITE);
            back.draw(g,"Back",15);
            companionHealth.erase(companionHealth.begin()+i);
            companionNames.erase(companionNames.begin()+i);
            diseaseState.erase(diseaseState.begin()+i);
            badnessLevel.erase(badnessLevel.begin()+i);
            numOfCompanions -= 1;
            while (g.draw())
            {
                for (const Event& e : g.events())
                {
                    if (e.evtType == EvtType::MousePress && back.isButtonPressed(e))
                    {
                        return true;
                    }
                }
            }


        }
    }
    return false;
}

void PlayerData::removeFromInventory(std::string itemToRemove, int quantityToRemove)
{
    for (int i = 0; i<inventory.size(); ++i)
    {
        if (inventory[i].itemName == itemToRemove)
        {
            inventory[i].quantity -= quantityToRemove;
            return;
        }
    }
}

void PlayerData::checkFood(Graphics& g)
{
    Button back = {{600,120},{690,200}};

    bool eatenToday = false;

    if (findItem("Cheese"))
    {
        removeFromInventory("Cheese",1);
        eatenToday = true;
    }

    if (!eatenToday)
    {
        for (int i = 0; i < companionHealth.size(); ++i)
        {
            companionHealth[i]--;
        }
        g.polygon({{295,100},{695,100},{695,300},{295,300}},WHITE,BLACK);
        back.draw(g, "Back", 15);
        string starving = ("Everyone is starving to death.");
        g.text(300,200,15,starving,WHITE);

        while(g.draw())
        {
            for (const Event& e : g.events())
            {
                if (e.evtType == EvtType::MousePress && back.isButtonPressed(e))
                {
                    return;
                }
            }
        }
    }
}

double PlayerData::changeRations(mssm::Graphics& g)
{
    Button poor = {{300,235},{380, 295}};
    Button normal = {{400,235},{480,295}};
    Button healthy = {{500,235},{580,295}};
    g.polygon({{295,100},{695,100},{695,300},{295,300}},WHITE,BLACK);
    g.text(300,125,20,"Change Rationing:",WHITE);

    poor.draw(g,"Poor",15);
    normal.draw(g,"Normal",15);
    healthy.draw(g,"Well Fed",15);

    while (g.draw())
    {
        for(const Event& e : g.events())
        {
            if (e.evtType == EvtType::MousePress)
            {
                if (poor.isButtonPressed(e))
                {
                    if (rations == 5)
                    {
                        return 1;
                    }
                    rations = 5;
                    return 2;
                }
                if (normal.isButtonPressed(e))
                {
                    if (rations == 3)
                    {
                        return 1;
                    }
                    rations = 3;
                    return 1;
                }
                if (healthy.isButtonPressed(e))
                {
                    if (rations == 0)
                    {
                        return 1;
                    }
                    rations = 0;
                    return 0.5;
                }
            }
        }
    }
}

void PlayerData::checkBrigands(Graphics &g, int chanceOfBrigands)
{
    Button pay = {{300,235},{380, 295}};
    Button giveUp = {{400,235},{480,295}};

    if (findItem("Knight"))
    {
        for (int i = 0; i < inventory.size(); ++i)
        {
            if (inventory[i].itemName == "Knight")
            {
                protection = inventory[i].quantity * 10;

                if (difficulty == 3)
                {
                    if (protection >= 70)
                    {
                        protection = 69;
                    }
                }
                if (difficulty == 2)
                {
                    if (protection >= 35)
                    {
                        protection = 34;
                    }
                }
            }
        }
    }

    int trueChance  = chanceOfBrigands - protection;
    int rand = g.randomInt(1,100);
    if (rand <= trueChance)
    {
        int toTake = g.randomInt(1,100);
        g.polygon({{295,100},{695,100},{695,300},{295,300}},WHITE,BLACK);
        string moneyOr = "Your Money or Your Life!|We Want ";
        moneyOr.append(to_string(toTake));
        moneyOr.append(" Pounds.");
        printTextTwo(g, 300,120,moneyOr,WHITE);
        if (protection != 0)
        {
            for (int i = 0; i < inventory.size(); ++i)
            {
                if (inventory[i].itemName == "Knight")
                {
                    inventory[i].quantity -= 1;
                    break;
                }
            }
        }
        if (money[0] >= toTake)
        {
            pay.draw(g,"Pay",15);
        }
        giveUp.draw(g,"Give Up",15);

        while (g.draw())
        {
            for (const Event& e : g.events())
            {
                if (e.evtType == EvtType::MousePress)
                {
                    if (pay.isButtonPressed(e) && money[0] >= toTake)
                    {
                        money[0] -= toTake;
                        return;
                    }
                    if (giveUp.isButtonPressed(e))
                    {
                        int ra = g.randomInt(0,3);
                        companionHealth[ra] -= 10;
                        return;
                    }
                }
            }
        }
    }
}

void PlayerData::checkForBadness(mssm::Graphics& g)
{
    Button back = {{600,120},{690,200}};

    int i = g.randomInt(0,numOfCompanions-1);

    int rand = g.randomInt(0, 15 - badnessLevel[i]);

    if (rand < 5)
    {
        string oopsy = companionNames[i];
        int problem = g.randomInt(0,5);
        g.polygon({{295,100},{695,100},{695,300},{295,300}},WHITE,BLACK);
        back.draw(g,"Back",15);
        switch (problem)
        {
        case 0:
            oopsy.append(" has broken their arm.");
            break;
        case 1:
            oopsy.append(" has broken their arm.");
            break;
        case 2:
            oopsy.append(" has broken their leg.");
            break;
        case 3:
            oopsy.append(" has contracted the plague.");
            break;
        case 4:
            oopsy.append(" is freezing to death.");
            break;
        case 5:
            oopsy.append(" has rickets, like the sailors of old.");
            break;
        }
        g.text(300,200,15,oopsy,RED);
        companionHealth[i] -= 1;
        diseaseState[i] = 10 - companionHealth[i];
        while (g.draw())
        {
            for (const Event& e : g.events())
            {
                if (e.evtType == EvtType::MousePress && back.isButtonPressed(e))
                {
                    return;
                }
            }
        }
    }
}

void printTextTwo(Graphics& g, int x, int y, string text, Color textColor)
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

int PlayerData::changePace(mssm::Graphics& g)
{
    Button horse = {{450,590},{570,650}};
    Button back = {{830,20},{950,80}};
    Button slow = {{30,590},{150,650}};
    Button normal = {{170,590},{290,650}};
    Button fast = {{310, 590},{430,650}};

    bool horses = false;

    g.clear();
    drawHUDs(g);
    if (findItem("Horses"))
    {
        horse.draw(g,"Horse",15);
        horses = true;
    }
    back.draw(g, "Back",15);
    slow.draw(g,"Slow",15);
    normal.draw(g,"Normal",15);
    fast.draw(g,"Fast",15);

    while(g.draw())
    {
        for(const Event& e : g.events())
        {
            if (e.evtType == EvtType::MousePress)
            {
                if (slow.isButtonPressed(e))
                {
                    travelPace = 0;
                    return 2;
                }
                if (normal.isButtonPressed(e))
                {
                    travelPace = 5;
                    return 5;
                }
                if (fast.isButtonPressed(e))
                {
                    travelPace = 10;
                    return 8; //8
                }
                if (horses && horse.isButtonPressed(e))
                {
                    travelPace = 5;
                    return 12;
                }
            }
        }
    }
}

bool PlayerData::findItem(string itemToFind)
{
    for (int i = 0; i < inventory.size(); ++i)
    {
        if (inventory[i].itemName == itemToFind)
        {
            return true;
        }
    }
    return false;
}

int getNumber(Graphics& g, double x, double y, double size)
{
    string name = "";

    while (g.draw())
    {
        for(const Event& e : g.events())
        {
            if (e.evtType == EvtType::KeyPress)
            {
                if (e.arg == 16777220 && name.size() != 0) //Enter
                {
                    int num;
                    stringstream stringToNum(name);

                    stringToNum >> num;

                    return num;
                }

                else if (e.arg == 48)
                {
                    g.text(x+(name.size() * size *.667), y, size, "0", WHITE);
                    name.push_back('0');
                }
                else if (e.arg == 49)
                {
                    g.text(x+(name.size() * size *.667), y, size, "1", WHITE);
                    name.push_back('1');
                }
                else if (e.arg == 50)
                {
                    g.text(x+(name.size() * size *.667), y, size, "2", WHITE);
                    name.push_back('2');
                }
                else if (e.arg == 51)
                {
                    g.text(x+(name.size() * size *.667), y, size, "3", WHITE);
                    name.push_back('3');
                }
                else if (e.arg == 52)
                {
                    g.text(x+(name.size() * size *.667), y, size, "4", WHITE);
                    name.push_back('4');
                }
                else if (e.arg == 53)
                {
                    g.text(x+(name.size() * size *.667), y, size, "5", WHITE);
                    name.push_back('5');
                }
                else if (e.arg == 54)
                {
                    g.text(x+(name.size() * size *.667), y, size, "6", WHITE);
                    name.push_back('6');
                }
                else if (e.arg == 55)
                {
                    g.text(x+(name.size() * size *.667), y, size, "7", WHITE);
                    name.push_back('7');
                }
                else if (e.arg == 56)
                {
                    g.text(x+(name.size() * size *.667), y, size, "8", WHITE);
                    name.push_back('8');
                }
                else if (e.arg == 57)
                {
                    g.text(x+(name.size() * size *.667), y, size, "9", WHITE);
                    name.push_back('9');
                }
                else
                {

                }

            }
        }
    }

    return 0;
}

void PlayerData::shop(mssm::Graphics& g, std::vector<Item>& shopInventory, std::string shopDesc)
{
    g.clear();
    g.text(15,18,18,shopDesc,WHITE);
    g.line(505,0,505,570, WHITE);
    g.polygon({{10,570},{970,570},{970,670},{10,670}}, WHITE, TRANSPARENT);
    g.polygon({{970,0},{g.width(), 0},{g.width(), 670},{970, 670}}, BLACK, BLACK);
    g.polygon({{10,670},{g.width(),670},{g.height(),g.width()},{10, g.height()}}, BLACK, BLACK);
    g.line(10,570,10,0, WHITE);
    g.line(970,570,970,0,WHITE);

    for(unsigned int i = 0; i<shopInventory.size(); i++)
    {
        string num = to_string(i+1);
        num.push_back('.');

        g.text(15,55 + (42*i),18, num,WHITE);
        g.text(30, 55 + (42*i), 18, shopInventory[i].itemName, WHITE);
    }

    string pounds = to_string(money[0]);
    pounds.append(" Pounds");
    string shillings = to_string(money[1]);
    shillings.append(" Shillings");
    string pennies = to_string(money[2]);
    pennies.append(" Pennies");
    string farthings = to_string(money[3]);
    farthings.append(" Farthings");

    g.text(400,70,18,pounds,WHITE);
    g.text(400,86,18,shillings,WHITE);
    g.text(400,102,18,pennies,WHITE);
    g.text(400,118,18,farthings,WHITE);

    Button doneShopping = {{400, 34}, {490,54}};
    doneShopping.draw(g, "Done", 12);

    while (g.draw())
    {

        pounds = to_string(money[0]);
        pounds.append(" Pounds");
        shillings = to_string(money[1]);
        shillings.append(" Shillings");
        pennies = to_string(money[2]);
        pennies.append(" Pennies");
        farthings = to_string(money[3]);
        farthings.append(" Farthings");


        for(const Event& e : g.events())
        {
            if (e.evtType == EvtType::KeyPress)
            {
                if (e.arg == 49)
                {
                    buyItem(g, shopInventory, 0, shopDesc);
                    doneShopping.draw(g, "Done", 12);

                }
                else if (e.arg == 50 && shopInventory.size()>1)
                {
                    buyItem(g, shopInventory, 1, shopDesc);
                    doneShopping.draw(g, "Done", 12);

                }
                else if (e.arg == 51 && shopInventory.size()>2)
                {
                    buyItem(g, shopInventory, 2, shopDesc);
                    doneShopping.draw(g, "Done", 12);

                }
                else if (e.arg == 52 && shopInventory.size()>3)
                {
                    buyItem(g, shopInventory, 3, shopDesc);
                    doneShopping.draw(g, "Done", 12);

                }
                else if (e.arg == 53 && shopInventory.size()>4)
                {
                    buyItem(g, shopInventory, 4, shopDesc);
                    doneShopping.draw(g, "Done", 12);

                }
                else if (e.arg == 54 && shopInventory.size()>5)
                {
                    buyItem(g, shopInventory, 5, shopDesc);
                    doneShopping.draw(g, "Done", 12);

                }
                else if (e.arg == 55 && shopInventory.size()>6)
                {
                    buyItem(g, shopInventory, 6, shopDesc);
                    doneShopping.draw(g, "Done", 12);

                }
                else if (e.arg == 56 && shopInventory.size()>7)
                {
                    buyItem(g, shopInventory, 7, shopDesc);
                    doneShopping.draw(g, "Done", 12);

                }
                else if (e.arg == 57 && shopInventory.size()>8)
                {
                    buyItem(g, shopInventory, 8, shopDesc);
                    doneShopping.draw(g, "Done", 12);

                }
                else
                {

                }
            }
            else if (e.evtType == EvtType::MousePress)
            {
                if (doneShopping.isButtonPressed(e))
                {
                    return;
                }
            }
        }
    }

}

void drawHUDs(Graphics& g)
{
    g.polygon({{10,570},{970,570},{970,670},{10,670}}, WHITE, BLACK);
    g.polygon({{970,0},{g.width(), 0},{g.width(), 670},{970, 670}}, BLACK, BLACK);
    g.polygon({{10,670},{g.width(),670},{g.height(),g.width()},{10, g.height()}}, BLACK, BLACK);
    g.line(10,570,10,0, WHITE);
    g.line(970,570,970,0,WHITE);
    return;
}

void PlayerData::cleanseInventory()
{
    for (int i = 0; i<inventory.size(); ++i)
    {
        if (inventory[i].quantity <= 0)
        {
            inventory.erase(inventory.begin()+i);
        }
    }
    return;
}

bool PlayerData::checkInventory(mssm::Graphics& g)
{
    Button back = {{830,20},{950,80}};

    g.clear();
    drawHUDs(g);
    back.draw(g, "Back",15);

    for(unsigned int i = 0; i<inventory.size(); i++)
    {
        if (inventory[i].quantity != 0)
        {
            string num = to_string(inventory[i].quantity);
            num.append(" .......... ");
            num.append(inventory[i].itemName);
            g.text(15,55 + (42*i),18, num,WHITE);
        }

    }

    while(g.draw())
    {
        for(const Event& e : g.events())
        {
            if(e.evtType == EvtType::MousePress)
            {
                if (e.arg == 16777216)
                {
                    g.setCloseOnExit(true);
                    return false;
                }
                if (back.isButtonPressed(e))
                {
                    return true;
                }
            }
        }

    }

}

void PlayerData::buyItem(mssm::Graphics& g, std::vector<Item>& shopInventory, int i, string shopDesc)
{
    g.text(515, 18, 18, shopInventory[i].description, WHITE);

    g.text(515, 55, 18, "How Many Do You Want?", WHITE);
    int numOfItems = getNumber(g,725,55,18);

    vector<int> totalPrice = {numOfItems*shopInventory[i].price[0],numOfItems*shopInventory[i].price[1],numOfItems*shopInventory[i].price[2],numOfItems*shopInventory[i].price[3]};

    string pounds = to_string(totalPrice[0]);
    string shillings = to_string(totalPrice[1]);
    string pennies = to_string(totalPrice[2]);
    string farthings = to_string(totalPrice[3]);

    string confirmation1 = "This will cost you ";
    confirmation1.append(pounds);
    confirmation1.append(" pounds, ");
    confirmation1.append(shillings);
    confirmation1.append(" shillings, ");
    string confirmation2 = pennies;
    confirmation2.append(" pennies, and ");
    confirmation2.append(farthings);
    confirmation2.append(" farthings.");
    string confirmation3 = "Do you wish to proceed? (y/n)";

    g.text(515, 97, 18, confirmation1, WHITE);
    g.text(515, 113, 18, confirmation2, WHITE);
    g.text(515, 145, 18, confirmation3, WHITE);

    string yOrN = getText(g, 785, 145, 18);

    if (yOrN == "y" || yOrN == "yes")
    {
        if(transaction(g, totalPrice))
        {
            bool passOver = false;
            Item toBeAdded = {shopInventory[i].itemName, numOfItems, shopInventory[i].price, shopInventory[i].description};
            for(int i = 0; i < inventory.size(); ++i)
            {
                if (inventory[i].itemName == toBeAdded.itemName)
                {
                    inventory[i].quantity = inventory[i].quantity + numOfItems;
                    passOver = true;
                }
            }
            if (!passOver)
            {
                inventory.push_back(toBeAdded);
            }
        }
    }
    else if (yOrN == "n" || yOrN == "no"){}
    else
    {
        g.text(200,400,18, "Bad Transaction", RED);
        g.draw(1000);
        g.clear();
    }

    g.clear();

    g.polygon({{10,570},{970,570},{970,670},{10,670}}, WHITE, TRANSPARENT);
    g.polygon({{970,0},{g.width(), 0},{g.width(), 670},{970, 670}}, BLACK, BLACK);
    g.polygon({{10,670},{g.width(),670},{g.height(),g.width()},{10, g.height()}}, BLACK, BLACK);
    g.line(10,570,10,0, WHITE);
    g.line(970,570,970,0,WHITE);

    string chaPounds = to_string(money[0]);
    chaPounds.append(" Pounds");
    string chaShillings = to_string(money[1]);
    chaShillings.append(" Shillings");
    string chaPennies = to_string(money[2]);
    chaPennies.append(" Pennies");
    string chaFarthings = to_string(money[3]);
    chaFarthings.append(" Farthings");

    g.text(400,70,18,chaPounds,WHITE);
    g.text(400,86,18,chaShillings,WHITE);
    g.text(400,102,18,chaPennies,WHITE);
    g.text(400,118,18,chaFarthings,WHITE);

    g.text(15,18,18,shopDesc,WHITE);

    g.line(505,0,505,570, WHITE);

    for(unsigned int i = 0; i<shopInventory.size(); i++)
    {
        string num = to_string(i+1);
        num.push_back('.');

        g.text(15,55 + (42*i),18, num,WHITE);
        g.text(30, 55 + (42*i), 18, shopInventory[i].itemName, WHITE);
    }

}

bool PlayerData::transaction(Graphics& g, vector<int> totalCharge)
{
    double totalChargeInPennies = totalCharge[2] + totalCharge[1]*12 + totalCharge[0]*240;

    double totalMoneyInPennies = money[2] + money[1]*12 + money[0]*240;

    int baseChange = totalMoneyInPennies - totalChargeInPennies;

    int newChange;

    if (totalChargeInPennies > totalMoneyInPennies)
    {
        g.text(515, 200, 18, "You don't have enough money for this...", WHITE);
        g.draw(1000);
        g.clear();
        return false;
    }

    else
    {
        int numPounds = baseChange / 240;
        newChange = baseChange - (240 * numPounds);

        int numShillings = newChange / 12;
        newChange = newChange - (12 * numShillings);

        int numPennies = newChange;

        money[0] = numPounds;
        money[1] = numShillings;
        money[2] = numPennies;
        return true;
    }
}

int readyToStart(mssm::Graphics& g)
{
    g.clear();

    Button startGame = {{20,50},{120,80}};
    Button changeNames = {{140,50},{240,80}};

    startGame.draw(g, "Start Game", 10);
    changeNames.draw(g, "Change Names", 10);

    while (g.draw())
    {
        for(const Event& e : g.events())
        {
            if (e.evtType == EvtType::MousePress)
            {
                if (startGame.isButtonPressed(e))
                {
                    return 1;
                }
                if (changeNames.isButtonPressed(e))
                {
                    return 3;
                }
            }
        }
    }

    return 0;
}

void PlayerData::endGame(Graphics& g)
{
    g.clear();
    g.text(50,50,50,"You Win!", PURPLE);

    while (g.draw())
    {
        for(const Event& e : g.events())
        {
            if (e.evtType == EvtType::KeyPress && e.arg == 16777216)
            {
                g.setCloseOnExit(true);
                return;
            }
        }
    }
}
