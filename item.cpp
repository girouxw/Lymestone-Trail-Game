#include "item.h"
#include <vector>
#include <string>
#include "graphics.h"
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace mssm;

Item::Item(std::string name, double quant, std::vector<int> priceTag, string desc)
{
    itemName = name;
    quantity = quant;
    price = priceTag;
    description = desc;
}
