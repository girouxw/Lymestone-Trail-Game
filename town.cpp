#include "town.h"

Town::Town(int townNum, std::string name, std::string townDescrip, std::vector<Item> shoppe, std::string shopDescrip)
{
    townNumber = townNum;
    townName = name;
    townDesc = townDescrip;
    shop = shoppe;
    shopDesc = shopDescrip;
}
