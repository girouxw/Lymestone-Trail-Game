#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include <string>
#include "graphics.h"
#include <sstream>
#include <fstream>
#include <algorithm>


class Item
{
public:
    std::string itemName;
    int quantity;
    std::vector<int> price;
    std::string description;
    bool isFood;

    Item(std::string item, double quant, std::vector<int> priceTag, std::string desc);
};

#endif // ITEM_H
