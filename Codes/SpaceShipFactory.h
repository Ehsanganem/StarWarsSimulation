//
// Created by ganim on 28/04/2024.
//

#ifndef FINALPROJECT_SPACESHIPFACTORY_H
#define FINALPROJECT_SPACESHIPFACTORY_H
#include <memory>
#include "SpaceShip.h"


class SpaceShipFactory {
public:
    enum class SpaceshipType { SHUTTLE, BOMBER, DESTROYER, FALCON };

    static std::shared_ptr<SpaceShip> createSpaceship(const std::string& spaceshipName,
        const Location& start, SpaceShipFactory::SpaceshipType type,
        const std::weak_ptr<ImperialAgent>& agent = std::weak_ptr<ImperialAgent>());
};


#endif //FINALPROJECT_SPACESHIPFACTORY_H
