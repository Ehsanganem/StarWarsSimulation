//
// Created by ganim on 18/04/2024.
//

#include "Model.h"
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "FortressStar.h"
#include "ConcreteAgentFactory.h"
#include "SpaceShipFactory.h"
#include "MillenniumFalcon.h"
#include "TieBomber.h"
#include "StarDestoyer.h"
#include "FreeMovementImperialAgentSpaceShip.h"

#define STATION_TYPE_STATION "station"
#define STATION_TYPE_FORTRESS "fortress"
#define DEFAULT_SIZE 25
#define DEFAULT_START_X 0
#define DEFAULT_START_Y 0
#define DEFAULT_SCALE 2.0
#define BASE_SCALE_UNIT_KM 1000

const std::map<std::string, AgentFactory::AgentType> Model::agentTypeMap = 
{        {"midshipman",AgentFactory::AgentType::MIDSHIPMAN}
        ,{"commander",AgentFactory::AgentType::COMMANDER},
         {"admiral",AgentFactory::AgentType::ADMIRAL} };

const std::map<std::string, SpaceShipFactory::SpaceshipType> Model::shipTypeMap =
{        {"bomber",SpaceShipFactory::SpaceshipType::BOMBER}
        ,{"falcon",SpaceShipFactory::SpaceshipType::FALCON},
         {"destroyer",SpaceShipFactory::SpaceshipType::DESTROYER},
         {"shuttle",SpaceShipFactory::SpaceshipType::SHUTTLE} };


// static instance
std::shared_ptr<Model> Model::instance = nullptr;


//this function is responsible for loading the stations from file
void Model::load(const std::string& file)
{
    std::ifstream stream(file);
    if (!stream) {
        throw std::invalid_argument("ERROR: file cannot be opened");
    }

    std::string line;

    while (std::getline(stream, line)) {
        // removes delimiters and wrappers: , ( )
        line.erase(std::remove(line.begin(), line.end(), ','), line.end());
        line.erase(std::remove(line.begin(), line.end(), '('), line.end());
        line.erase(std::remove(line.begin(), line.end(), ')'), line.end());

        std::string type;
        std::string name;
        double location_x, location_y;
        int num_containers;
        int produtcion;

        std::stringstream sstream(line);

        // reach each line data
        if (sstream >> type >> name >> location_x >> location_y >> num_containers >> produtcion) 
        {
            if (type != STATION_TYPE_STATION && type != STATION_TYPE_FORTRESS) {
                throw std::invalid_argument("invalid station type");
            }
            std::shared_ptr<Station> station;
            if (type == STATION_TYPE_STATION) {
                station = std::make_shared<Station>(name, Location(location_x, location_y), 
                    num_containers, produtcion);
            }
            else {
                station = std::make_shared<FortressStar>(name, Location(location_x, location_y), 
                    num_containers);
            }

            stations[name] = station;
        }
        else 
        {
            throw std::invalid_argument("invalid file line");
        }

    }

    std::stringstream sstream(line);
}

Model::Model() :time(0),
    agentFactory(std::unique_ptr<ConcreteAgentFactory>(new ConcreteAgentFactory())),
    ships(), agents(), stations() 
{
    // default station
    stations["DS"] = std::make_shared<FortressStar>("DS", Location(40.0, 10.0),
        100000);
}

std::weak_ptr<Model> Model::getInstance(const std::string& file) {
    if (!instance) {
        // using new as std::make_shared requires ctor to be public
        // (made private for singleton impelmentation)
        instance = std::shared_ptr<Model>(new Model());
        instance->load(file);
    }

    return instance;
}

std::weak_ptr<ImperialAgent> Model::getAgent(const std::string& name) {
    return agents.at(name);
}

const std::map<std::string, std::shared_ptr<Station>>& Model::getStations() const {
    return stations;
}

const std::map<std::string, std::shared_ptr<SpaceShip>>& Model::getShips() const {
    return ships;
}

void Model::go() {
    for (const auto& pair : ships) // move ships
    {
        pair.second->move();
    }
    for (const auto& pair : stations) // make relevant stations produce
    {
        pair.second->go();
    }
    time++;
}

void Model::stop(const std::string& shipName)
{
    if (ships.count(shipName) == 0) // no ship with this name found
    {
        throw std::invalid_argument("ship doesn't exist");
    }

    ships[shipName]->stop();
}

std::weak_ptr<SpaceShip>
Model::createSpaceship(SpaceShipFactory::SpaceshipType type, const std::string& shipName,
    const Location& cordinates,
    const std::string& pilotName)
{
    if (shipName.empty())
    {
        throw std::invalid_argument("ship name must not be empty");
    }
    if (ships.count(shipName) != 0) // ship with this name found
    {
        throw std::invalid_argument("ship already exists");
    }

    std::weak_ptr<ImperialAgent> agent;

    if (type != SpaceShipFactory::SpaceshipType::FALCON) // nonfalcon requires a pilot
    {
        if (agents.count(pilotName) == 0) // no agent with this name found
        {
            throw std::invalid_argument("ERROR: pilot doesn't exist.");
        }

        agent = agents.at(pilotName);
    }

    ships[shipName] = SpaceShipFactory::createSpaceship(shipName, cordinates, type, agent);

    return ships[shipName];
}

std::shared_ptr<ImperialAgent> Model::createAgent(const std::string& agentName,
    AgentFactory::AgentType type) 
{
    if (agentName.empty())
    {
        throw std::invalid_argument("ERROR: Agent name must not be empty");
    }
    if (agents.count(agentName) > 0) // agent with this name found
    {
        throw std::invalid_argument("ERROR: Agent already exists");
    }

    agents[agentName] = agentFactory->getAgent(type, agentName);

    return agents[agentName];
}

void Model::setCourse(const std::string& shipName, double angle, double speed)
{
    if (ships.count(shipName) == 0) // no ship with this name found
    {
        throw std::invalid_argument("ERROR: ship doesn't exist");
    }

    std::shared_ptr<MillenniumFalcon> falcon 
        = std::dynamic_pointer_cast<MillenniumFalcon>(ships[shipName]);

    if (falcon) // falcon has a course with speed
    {
        falcon->course(angle, speed);
    }
    else
    {
        // check if ship is able to have its course set
        std::shared_ptr<FreeMovementImperialAgentSpaceShip> freeMovementShip
             = std::dynamic_pointer_cast<FreeMovementImperialAgentSpaceShip>(ships[shipName]);

        if (freeMovementShip)
        {
            freeMovementShip->course(angle);
        }
        else
        {
            throw std::invalid_argument("ERROR: ship of this type cannot set course");
        }
    }
}

void Model::setPosition(const std::string& shipName, const Location& pos, double speed)
{
    if (ships.count(shipName) == 0)
    {
        throw std::invalid_argument("ERROR: Spaceship doesn't exist");
    }

    std::shared_ptr<MillenniumFalcon> falcon
        = std::dynamic_pointer_cast<MillenniumFalcon>(ships[shipName]);

    if (falcon) // falcon has a position with speed
    {
        falcon->position(pos, speed);
    }
    else
    {
        // check if ship is able to have its position set
        std::shared_ptr<FreeMovementImperialAgentSpaceShip> freeMovementShip
            = std::dynamic_pointer_cast<FreeMovementImperialAgentSpaceShip>(ships[shipName]);

        if (freeMovementShip)
        {
            freeMovementShip->position(pos);
        }
        else
        {
            throw std::invalid_argument("ERROR: ship of this type cannot set position.");
        }
    }
}

void Model::setDestination(const std::string& shipName, const std::string& siteName, double speed)
{
    if (ships.count(shipName) == 0) // no ship with this name found
    {
        throw std::invalid_argument("ERROR: spaceship doesn't exist.");
    }
    if (stations.count(siteName) == 0) // no station with this name found
    {
        throw std::invalid_argument("ERROR: station doesn't exist.");
    }
    std::shared_ptr<MillenniumFalcon> falcon
        = std::dynamic_pointer_cast<MillenniumFalcon>(ships[shipName]);

    if (falcon) // falcon has a destination with speed
    {
        falcon->destination(stations.at(siteName), speed);
    }
    else
    {
        // check if ship is able to have its destination set
        std::shared_ptr<FreeMovementImperialAgentSpaceShip> freeMovementShip
            = std::dynamic_pointer_cast<FreeMovementImperialAgentSpaceShip>(ships[shipName]);

        if (freeMovementShip)
        {
            freeMovementShip->destination(stations.at(siteName));
        }
        else
        {
            throw std::invalid_argument("ERROR: ship of this type cannot set destination.");
        }
    }
}

void Model::startSupply(const std::string& shipName, const std::string& stationName,
    const std::string& fortressStarName)
{
    if (ships.count(shipName) == 0) // no ship with this name found
    {
        throw std::invalid_argument("ERROR: Ship doesn't exist");
    }

    std::shared_ptr<Shuttle> shuttle =
        std::dynamic_pointer_cast<Shuttle>(ships.at(shipName));

    if (!shuttle) // not a shuttle
    {
        throw std::invalid_argument("ERROR: Supply ship is not a shuttle.");
    }
    if (stations.count(stationName) == 0) // no station with this name found
    {
        throw std::invalid_argument("ERROR: source station doesn't exist.");
    }
    if (stations.count(fortressStarName) == 0) // no station with this name found
    {
        throw std::invalid_argument("ERROR: destination fortress star doesn't exist.");
    }

    std::shared_ptr<FortressStar> fortressStar =
        std::dynamic_pointer_cast<FortressStar>(stations.at(fortressStarName));

    if (!fortressStar) // destination is not a fortress
    {
        throw std::invalid_argument("ERROR: destination station is not a fortress star.");
    }

    shuttle->startSupply(stations.at(stationName), fortressStar);
}

bool Model::attack(const std::string& shipName, const std::string& attackedSpaceshipName)
{
    if (ships.count(shipName) == 0) // no ship with this name found
    {
        throw std::invalid_argument("ERROR: Attacker ship doesn't exist.");
    }
    if (ships.count(attackedSpaceshipName) == 0) // no ship with this name found
    {
        throw std::invalid_argument("ERROR: Attacked ship doesn't exist.");
    }

    std::shared_ptr<MillenniumFalcon> falcon 
        = std::dynamic_pointer_cast<MillenniumFalcon>(ships[shipName]);

    if (!falcon) // attacker is not a falcon
    {
        throw std::invalid_argument("Attacker ship is not a falcon.");
    }
    std::shared_ptr<Shuttle> shuttle
        = std::dynamic_pointer_cast<Shuttle>(ships[attackedSpaceshipName]);

    if (!shuttle) // attacker is not a shuttle
    {
        throw std::invalid_argument("Attacked ship is not a shuttle.");
    }

    return falcon->attack(shuttle);
}

void Model::shoot(const std::string& shipName, const Location& location) 
{
    if (ships.count(shipName) == 0) // no ship with this name found
    {
        throw std::invalid_argument("Ship doesn't exist");
    }

    std::shared_ptr<StarDestoyer> destoyer
        = std::dynamic_pointer_cast<StarDestoyer>(ships[shipName]);

    if (!destoyer) 
    {
        throw std::invalid_argument("shooter is not a StarDestoyer");
    }

    destoyer->shoot(location);
}

int Model::getBaseScaleUnitKm() const
{
    return BASE_SCALE_UNIT_KM;
}

int Model::getTime() const 
{
    return this->time;
}

void Model::setTime(int time) 
{
    this->time = time;
}

void Model::status() const
{
    for (const auto& pair : stations) {
        std::cout << *pair.second << std::endl;
    }
    for (const auto& pair : ships) {
        std::cout << *pair.second << std::endl;
    }
}




