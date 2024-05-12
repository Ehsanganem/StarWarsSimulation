//
// Created by ganim on 15/04/2024.
//

#include "StarDestoyer.h"
#include "Admiral.h"
#include <stdexcept>
#include "Utilis.h"
#include "Model.h"
#include "MillenniumFalcon.h"

#define SPEED 2.0
#define ROCKET_SPEED 3.0

StarDestoyer::Rocket::Rocket(const Location& location)
    : FreeMovementImperialAgentSpaceShip(location, getName(), getImperialAgent(), ROCKET_SPEED),
    lastLocation()
{
}

StarDestoyer::StarDestoyer(const Location& location, const std::string& name,
    const std::weak_ptr<ImperialAgent>& imperialAgent)
    : FreeMovementImperialAgentSpaceShip(location, name, imperialAgent, SPEED),
    rockets()
{
    if (!std::dynamic_pointer_cast<Admiral>(imperialAgent.lock())) 
    {
        throw std::invalid_argument("Error: pilot must be Admiral.");
    }
}

void StarDestoyer::Rocket::move()
{
    lastLocation = std::make_shared<Location>(getLocation());
    moveOneStep();

    if (getFlightMode() == SpaceShip::FlightMode::STOPPED)
    {
        // reached position
        for (const auto& pair : Model::getInstance().lock()->getShips()) 
        {
            std::shared_ptr<MillenniumFalcon> falcon 
                = std::dynamic_pointer_cast<MillenniumFalcon>(pair.second);

            if (falcon && falcon->getFlightMode() != SpaceShip::FlightMode::DEAD
                && (Utilis::doubleIsEqual(0.0, falcon->getLocation().distance(getLocation()))
                    || Utilis::doubleIsEqual(0.0, 
                        falcon->getLocation().distance(getLocation().average(*lastLocation))))) 
            {
                // rocket hit a falcon
                falcon->die();
                break;
            }
        }
    }
}

std::string StarDestoyer::Rocket::getType() const
{
    return "rocket";
}

void StarDestoyer::shoot(const Location& position)  
{
    Rocket rocket(getLocation());
    rocket.position(position);
    rockets.push_back(rocket);
}

void StarDestoyer::move() 
{
    moveOneStep();

    // launched rockets continue moving even if ship is dead
    std::vector<Rocket> remainingRockets;

    for (Rocket& rocket : rockets)
    {
        rocket.move();

        // retain rockets which did not reach position
        if (rocket.getFlightMode() == SpaceShip::FlightMode::MOVING) 
        {
            remainingRockets.push_back(rocket);
        }
    }

    rockets = remainingRockets;
}

std::string StarDestoyer::getType() const
{
    return "destroyer";
}


