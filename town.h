#ifndef TOWN_H
#define TOWN_H

#include <vector>
#include <string>
#include "graphics.h"
#include "item.h"

class Town
{
public:
    int townNumber;
    std::string townName;
    std::string townDesc;
    std::vector<Item> shop;
    std::string shopDesc;
    std::vector<int> canGoTo;
    int dangerLevel;
    int numOfScreens;
    mssm::Image townPic;





    Town(int townNum, std::string name, std::string townDescrip, std::vector<Item> shoppe,std::string shopDescrip, std::vector<int> placesToGo, int dangerLev,int numberOfScreens, mssm::Image townPicture);
};

#endif // TOWN_H
