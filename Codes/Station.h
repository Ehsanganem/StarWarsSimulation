//
// Created by ganim on 15/04/2024.
//

#ifndef FINALPROJECT_STATION_H
#define FINALPROJECT_STATION_H

#include <string>
#include "Location.h"

class Station {
private:
    std::string name;
    Location location;
    int productionRate;

protected:
    int amount;
    virtual std::string getType() const;
public:
    Station(const std::string& name,const Location& location,int amount,const int& production);
    const Location& getLocation() const;
    int getAmount() const;
    const std::string& getName() const;
    virtual void go();
    int unloadContainers(int maxAmount); // returns number of containers unloaded
    friend std::ostream & operator<<(std::ostream & out,const Station& stat);
};


#endif //FINALPROJECT_STATION_H
