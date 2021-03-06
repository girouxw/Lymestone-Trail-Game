#include "town.h"

Town::Town(int townNum, std::string name, std::string townDescrip, std::vector<Item> shoppe, std::string shopDescrip, std::vector<int> placesToGo, int dangerLev, int numberOfScreens, mssm::Image townPicture)
{
    townNumber = townNum;
    townName = name;
    townDesc = townDescrip;
    shop = shoppe;
    shopDesc = shopDescrip;
    canGoTo = placesToGo;
    dangerLevel = dangerLev;
    numOfScreens = numberOfScreens;
    townPic = townPicture;
}
