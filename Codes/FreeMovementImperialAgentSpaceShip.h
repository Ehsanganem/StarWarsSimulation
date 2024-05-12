//
// Created by ganim on 28/04/2024.
//

#ifndef FINALPROJECT_FREEMOVEMENTIMPERIALAGENTSPACESHIP_H
#define FINALPROJECT_FREEMOVEMENTIMPERIALAGENTSPACESHIP_H

#include "SpaceShip.h"
#include "ImperialAgentSpaceShip.h"
#include <memory>
#include "Location.h"
#include "ImperialAgent.h"

// represents a ship which can have its position, destination and course set
class FreeMovementImperialAgentSpaceShip : public ImperialAgentSpaceShip
{
public:
    FreeMovementImperialAgentSpaceShip(const Location& location, const std::string& name,
        const std::weak_ptr<ImperialAgent>& imperialAgent, double speed);

    virtual void position(const Location& position);
    virtual void destination(const std::weak_ptr<Station>& destination);
    virtual void course(double angle);
};

#endif // !FINALPROJECT_FREEMOVEMENTIMPERIALAGENTSPACESHIP_H