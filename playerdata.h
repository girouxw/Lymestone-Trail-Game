#ifndef PLAYERDATA_H
#define PLAYERDATA_H

#include <vector>
#include <string>
#include "graphics.h"
#include "item.h"
#include "town.h"



class PlayerData
{
public:
    double numOfCompanions;
    std::vector<std::string> companionNames;
    std::vector<double> companionHealth;
    std::vector<int> money; // Has 4 values; Pounds, Shillings, Pence, Farthing in that order
    std::vector<Item> inventory;
    int difficulty;
    bool diseased;
    int pathNumber;
    int townNumber;

    PlayerData(double numberOfCompanions, std::vector<double> startingHealth, std::vector<int> startingMoney);
    void drawCharacter(mssm::Graphics& g, bool walkCycleVal);
    void shop(mssm::Graphics& g, std::vector<Item>& shopInventory, std::string shopDesc);
    void buyItem(mssm::Graphics& g, std::vector<Item>& shopInventory, int i, std::string shopDesc);
    bool transaction(mssm::Graphics& g, std::vector<int> totalCharge);
    void selectDifficulty(mssm::Graphics& g);
    void selectCharacters(mssm::Graphics& g);
    bool titleScreen(mssm::Graphics& g, double& initialWidth, double& initialHeight);
    void selectPath(mssm::Graphics& g);
    bool checkInventory(mssm::Graphics& g);
    int selectDestination(mssm::Graphics& g, std::vector<Town> allTowns);
};




void printTextTwo(mssm::Graphics& g, int x, int y, std::string text, mssm::Color textColor);
std::string getText(mssm::Graphics& g, double x, double y, double size);
int getNumber(mssm::Graphics& g, double x, double y, double size);
int readyToStart(mssm::Graphics& g);
void drawHUDs(mssm::Graphics& g);





#endif // PLAYERDATA_H
