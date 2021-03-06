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
    std::vector<int> companionHealth;
    std::vector<int> money; // Has 4 values; Pounds, Shillings, Pence, Farthing in that order
    std::vector<Item> inventory;
    int difficulty;
    std::vector<int> diseaseState;
    int townNumber;
    int travelPace;
    std::vector<int> badnessLevel;
    std::vector<mssm::Image> walkingSprites;
    int spriteNumber;
    int spriteIterator;
    int rations;
    double protection;

    PlayerData(double numberOfCompanions, std::vector<int> startingHealth, std::vector<int> startingMoney, std::vector<mssm::Image> walks);
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
    void checkForBadness(mssm::Graphics& g);
    void printHealth(mssm::Graphics& g);
    int changePace(mssm::Graphics& g);
    void stopToRest(mssm::Graphics& g);
    bool checkForDeath(mssm::Graphics& g);
    void checkFood(mssm::Graphics& g);
    mssm::Image updateSprite();
    double changeRations(mssm::Graphics& g);
    void endGame (mssm::Graphics& g);
    bool findItem(std::string itemToFind);
    void removeFromInventory(std::string itemToRemove, int quantityToRemove);
    void cleanseInventory();
    void checkBrigands(mssm::Graphics& g, int chanceOfBrigands);
    void conversation(mssm::Graphics& g);
};




void printTextTwo(mssm::Graphics& g, int x, int y, std::string text, mssm::Color textColor, int size);
std::string getText(mssm::Graphics& g, double x, double y, double size);
int getNumber(mssm::Graphics& g, double x, double y, double size);
int readyToStart(mssm::Graphics& g);
void drawHUDs(mssm::Graphics& g);





#endif // PLAYERDATA_H
