//
// Created by ganim on 15/04/2024.
//

#include "Station.h"
#include <algorithm>

Station::Station(const std::string& name, const Location& location, int amount,
    const int& productionRate) :name(name), location(location), 
    productionRate(productionRate), amount(amount)
{
}

const Location& Station::getLocation() const 
{
    return this->location;
}

int Station::getAmount() const
{
    return this->amount;
}

int Station::unloadContainers(int maxAmount)
{
    int unloadAmount = std::min(maxAmount, amount);
    amount -= unloadAmount;
    return unloadAmount;
}

const std::string& Station::getName() const 
{
    return this->name;
}

void Station::go()
{
    amount += productionRate;
}

std::string Station::getType() const 
{
    return "station";
}

std::ostream& operator<<(std::ostream& out, const Station& stat)
{
    return out << stat.getType() << " " << stat.name << " at position " 
        << stat.location << ", Inventory: "  << stat.amount;
}
