//
// Created by ganim on 28/04/2024.
//

#include "SpaceShipFactory.h"
#include "Shuttle.h"
#include "TieBomber.h"
#include "StarDestoyer.h"
#include "MillenniumFalcon.h"

std::shared_ptr<SpaceShip>
SpaceShipFactory::createSpaceship(const std::string& spaceshipName,
    const Location& start, SpaceShipFactory::SpaceshipType type,
    const std::weak_ptr<ImperialAgent>& agent)
{
    switch (type)
    {
    case SpaceShipFactory::SpaceshipType::SHUTTLE:
        return std::make_shared<Shuttle>(start, spaceshipName, agent);
    case SpaceShipFactory::SpaceshipType::BOMBER:
        return std::make_shared<TieBomber>(start, spaceshipName, agent);
    case SpaceShipFactory::SpaceshipType::DESTROYER:
        return std::make_shared<StarDestoyer>(start, spaceshipName, agent);
    case SpaceShipFactory::SpaceshipType::FALCON:
        return std::make_shared<MillenniumFalcon>(start, spaceshipName);
    }

    return std::shared_ptr<SpaceShip>();
}

