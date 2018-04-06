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


PlayerData::PlayerData(double numberOfCompanions, std::vector<double> startingHealth, std::vector<int> startingMoney)
{
    numOfCompanions = numberOfCompanions;
    companionHealth = startingHealth;
    money = startingMoney;
    companionNames = {" "," "," "," "};
    inventory = {};
    difficulty = -1;
}

bool PlayerData::titleScreen(mssm::Graphics& g, double& initialWidth, double& initialHeight)
{
    double width = g.width();
    double height = g.height();
    string stringWidth = to_string(width);
    string stringHeight = to_string(height);

    vector<Item> startingShop = {{"Wheel", 20,{1,0,0,0}, "A simple Wheel"}, {"Axel", 21, {0,2,10,0}, "A wooden Axel used in carts and wagons"}};
    string startingShopDesc = "Welcome to Flynn's General Store! Buy Something Will 'Ya!";

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
        g.line({1024,0},{1024,height}, WHITE);
        g.line({0,768},{width,768}, WHITE);

        Button startButton = {{20,130},{185,165}};
        Button exitButton = {{200,130},{365,165}};

        exitButton.draw(g, "Exit Game", 15);
        startButton.draw(g, "Start Game", 15);

        int p;

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
                        selectDifficulty(g);
                        selectCharacters(g);
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
            case 2:
                g.clear();
                shop(g, startingShop, startingShopDesc);
                break;
            case 3:
                companionNames[0] = "";
                companionNames[1] = "";
                companionNames[2] = "";
                companionNames[3] = "";
                selectCharacters(g);
                break;
            case 4:
                selectDifficulty(g);
                break;
            }
        }

    }
}

void PlayerData::selectDifficulty(mssm::Graphics& g)
{
    g.clear();
    g.draw();
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
                    difficulty = 0;
                    return;
                }
                if (aristocraftDiff.isButtonPressed(e))
                {
                    difficulty = 1;
                    return;
                }
                if (villaniDiff.isButtonPressed(e))
                {
                    difficulty = 2;
                    return;
                }
                if (poorPeasantDiff.isButtonPressed(e))
                {
                    difficulty = 3;
                    return;
                }
            }
        }
    }
}


void PlayerData::selectCharacters(mssm::Graphics& g)
{
    g.clear();
    g.draw();
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
                if (e.arg == 16777220) //Enter
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
}


void PlayerData::shop(mssm::Graphics& g, std::vector<Item>& shopInventory, std::string shopDesc)
{
    g.text(15,18,18,shopDesc,WHITE);
    g.line(g.width()/2,0,g.width()/2,g.height(), WHITE);

    for(int i = 0; i<shopInventory.size(); i++)
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
    g.text(515, 129, 18, confirmation3, WHITE);

    string yOrN = getText(g, 785, 129, 18);

    if (yOrN == "y" || yOrN == "yes")
    {
        if(transaction(g, totalPrice))
        {
            Item toBeAdded = {shopInventory[i].itemName, numOfItems, shopInventory[i].price, shopInventory[i].description};
            inventory.push_back(toBeAdded);
        }
    }
    else if (yOrN == "n" || yOrN == "no")
    {

    }
    else
    {
        g.text(200,400,18, "Bad Transaction", RED);
        g.draw(1000);
        g.clear();
    }

    g.clear();

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

    g.line(g.width()/2,0,g.width()/2,g.height(), WHITE);

    for(int i = 0; i<shopInventory.size(); i++)
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
    Button visitShop = {{140,50},{240,80}};
    Button changeNames = {{260,50},{360,80}};
    Button changeDiff = {{380,50},{480,80}};

    startGame.draw(g, "Start Game", 10);
    visitShop.draw(g, "Shop", 10);
    changeNames.draw(g, "Change Names", 10);
    changeDiff.draw(g, "Set Difficulty", 10);

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
                if (visitShop.isButtonPressed(e))
                {
                    return 2;
                }
                if (changeNames.isButtonPressed(e))
                {
                    return 3;
                }
                if (changeDiff.isButtonPressed(e))
                {
                    return 4;
                }
            }
        }
    }

}

































