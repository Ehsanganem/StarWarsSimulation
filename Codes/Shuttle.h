//
// Created by ganim on 15/04/2024.
//

#ifndef FINALPROJECT_SHUTTLE_H
#define FINALPROJECT_SHUTTLE_H

#include <string>
#include "SpaceShip.h"
#include "Station.h"
#include "FortressStar.h"
#include "ImperialAgentSpaceShip.h"
#include <vector>

class Shuttle : public ImperialAgentSpaceShip {
private:
    enum class SupplyMode { MOVING_TO_DEST, UNLOADING, MOVING_TO_SRC, LOADING, DONE };

    // structure to hold each supply command
    struct SupplyCommand 
    {
        std::weak_ptr<Station> loadStation;
        std::weak_ptr<FortressStar> unloadFortressStar;
    };

    int defenseUnits;
    int numContainers;
    SupplyMode supplyMode;
    std::vector<SupplyCommand> supplyCommands; // holds all supply commands not yet executed

    void endSupplyCommand();
    void reset();
public:
    Shuttle(const Location& location, const std::string& name,
        const std::weak_ptr<ImperialAgent>& imperialAgent);
    void startSupply(const std::weak_ptr<Station>& station,
        const std::weak_ptr<FortressStar>& star);
    void move() override;
    void stop() override;
    void die() override;
    void print(std::ostream& out) const override;
    int getDefenseUnit() const;
    void getAttacked(bool attackSuccess);
    std::string getType() const override;
};


#endif //FINALPROJECT_SHUTTLE_H
