//
// Created by ganim on 15/04/2024.
//

#include "Shuttle.h"
#include "ImperialAgent.h"
#include "MidShipMan.h"
#include <algorithm>
#include <stdexcept>

#define MAX_NUM_CONTAINERS 5
#define INITIAL_DEFENCE_UNITS 10
#define SPEED 0.5

Shuttle::Shuttle(const Location& location, const std::string& name,
    const std::weak_ptr<ImperialAgent>& imperialAgent)
    : ImperialAgentSpaceShip(location, name, imperialAgent, SPEED),
    defenseUnits(INITIAL_DEFENCE_UNITS),
    numContainers(0), supplyMode(Shuttle::SupplyMode::DONE), supplyCommands()
{
    if (!std::dynamic_pointer_cast<MidShipMan>(imperialAgent.lock())) {
        throw std::invalid_argument("ERROR: shuttle pilot must be MidShipMan.");
    }
}

void Shuttle::endSupplyCommand()
{
    numContainers = 0;
    supplyMode = Shuttle::SupplyMode::DONE;
    
    if (supplyCommands.size() > 0) // finished a command - remove it from queue
    {
        supplyCommands.erase(supplyCommands.begin());
    }
    if (getFlightMode() != SpaceShip::FlightMode::DEAD &&
        supplyCommands.size() > 0) // still more commands to execute
    {
        supplyMode = SupplyMode::MOVING_TO_SRC;
        setDestination(supplyCommands[0].loadStation);
    }
}

void Shuttle::reset()
{
    supplyMode = SupplyMode::DONE;
    numContainers = 0;
    supplyCommands.clear();
}

void Shuttle::startSupply(const std::weak_ptr<Station>& station,
    const std::weak_ptr<FortressStar>& star)
{
    if (getFlightMode() == SpaceShip::FlightMode::DEAD)
    {
        throw std::logic_error("Dead shuttle cannot start supply");
    }

    supplyCommands.push_back({ station, star }); // add to queue
    if (supplyMode == SupplyMode::DONE)
    {
        // start if no supply command in progresss
        supplyMode = SupplyMode::MOVING_TO_SRC;
        setDestination(station);
    }  
}

void Shuttle::move()
{
    if (getFlightMode() == SpaceShip::FlightMode::DEAD)
    {
        return;
    }

    switch (supplyMode) {
    case Shuttle::SupplyMode::DONE:
        // do nothing on DONE
        break;
    case Shuttle::SupplyMode::MOVING_TO_SRC:
        moveOneStep();

        if (getFlightMode() == SpaceShip::FlightMode::DOCKED) // done moving to src
        {
            supplyMode = Shuttle::SupplyMode::LOADING;
        }

        break;
    case Shuttle::SupplyMode::LOADING:
        // one time unit waited
        numContainers = supplyCommands[0].loadStation.lock()->unloadContainers(MAX_NUM_CONTAINERS);
        supplyMode = Shuttle::SupplyMode::MOVING_TO_DEST;
        setDestination(supplyCommands[0].unloadFortressStar);
        break;
    case Shuttle::SupplyMode::MOVING_TO_DEST:
        moveOneStep();

        if (getFlightMode() == SpaceShip::FlightMode::DOCKED) // done moving to dest
        {
            supplyMode = Shuttle::SupplyMode::UNLOADING;
        }
        break;
    case Shuttle::SupplyMode::UNLOADING:
        // one time unit waited
        supplyCommands[0].unloadFortressStar.lock()->loadContainer(numContainers);
        endSupplyCommand();
        break;
    }
}

void Shuttle::stop()
{
    SpaceShip::stop();
    reset();
}

void Shuttle::die()
{
    SpaceShip::die();
    reset();
}

void Shuttle::print(std::ostream& out) const
{
    SpaceShip::print(out);
    out << ", number of containers: " << numContainers << ", Strength: " << defenseUnits;
}

int Shuttle::getDefenseUnit() const {
    return defenseUnits;
}

void Shuttle::getAttacked(bool attackSuccess) {
    defenseUnits--;
 
    if (attackSuccess)
    {
        stop();
        endSupplyCommand();
    }
    if (defenseUnits == 0)
    {
        die();
    }
}

std::string Shuttle::getType() const 
{
    return "shuttle";
}
