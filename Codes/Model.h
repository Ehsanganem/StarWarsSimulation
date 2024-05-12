//
// Created by ganim on 18/04/2024.
//

#ifndef FINALPROJECT_MODEL_H
#define FINALPROJECT_MODEL_H

#include "Location.h"
#include "Shuttle.h"
#include "Station.h"
#include <memory>
#include <vector>
#include <map>
#include <istream>
#include <fstream>
#include "SpaceShipFactory.h"
#include "AgentFactory.h"

class Model {
private:
  /*  enum class ShipType { SHUTTLE, BOMBER, DESTROYER, FALCON };
    enum class AgentType { MIDSHIPMAN, COMMANDER, ADMIRAL };*/

    static std::shared_ptr<Model> instance;

    int time;
    std::unique_ptr<AgentFactory> agentFactory;

    std::map<std::string, std::shared_ptr<SpaceShip>> ships;
    std::map<std::string, std::shared_ptr<ImperialAgent>> agents;
    std::map<std::string, std::shared_ptr<Station>> stations;
    Model(); // private ctor for singleton


public:
    static const std::map<std::string, AgentFactory::AgentType> agentTypeMap;
    static const std::map<std::string, SpaceShipFactory::SpaceshipType> shipTypeMap;

    static std::weak_ptr<Model> getInstance(const std::string& file = "");

    void load(const std::string& file);
    int getBaseScaleUnitKm() const;
    int getTime()const;
    void setTime(int time);
    void go();
    void stop(const std::string& shipName);
    void shoot(const std::string& shipName, const Location& location);
    bool attack(const std::string& shipName, const std::string& attackedSpaceshipName);
    void setPosition(const std::string& shipName, const Location& pos, double speed = 0.0);
    void setDestination(const std::string& shipName, const std::string& siteName, 
        double speed = 0.0);
    void startSupply(const std::string& shipName, const std::string& stationName,
        const std::string& fortressStarName);
    const std::map<std::string, std::shared_ptr<SpaceShip>>& getShips() const;
    const std::map<std::string, std::shared_ptr<Station>>& getStations() const;

    std::weak_ptr<SpaceShip> createSpaceship(SpaceShipFactory::SpaceshipType type, 
        const std::string& shipName,
        const Location& cordinates,
        const std::string& pilotName = "");
    std::shared_ptr<ImperialAgent> createAgent(const std::string& agentName,
        AgentFactory::AgentType type);
    std::weak_ptr<ImperialAgent> getAgent(const std::string& name);


    void setCourse(const std::string& shipName, double angle, double speed = 0.0);
    void status() const;
   
};


#endif //FINALPROJECT_MODEL_H
