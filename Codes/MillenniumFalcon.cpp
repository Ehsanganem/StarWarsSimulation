//
// Created by ganim on 15/04/2024.
//

#include "MillenniumFalcon.h"
#include "Model.h"
#include "TieBomber.h"
#include <stdexcept>

#define DEFAULT_SPEED MAX_SPEED
#define INITIAL_ATTACK_UNITS 5
#define MIN_ATTACK_DISTANCE_SHUTTLE 0.1
#define FAIL_ATTACK_DISTANCE_BOMBER 0.25

void MillenniumFalcon::validateSpeed(double speed)
{
    if (speed > MAX_SPEED)
    {
        throw std::invalid_argument("Speed must be at most " 
            + std::to_string(Model::getInstance().lock()->getBaseScaleUnitKm() * MAX_SPEED)
            + "km/h");
    }
}

MillenniumFalcon::MillenniumFalcon(const Location& location, const std::string& name, double speed)
    : SpaceShip(location, name, DEFAULT_SPEED),
    attackUnits(INITIAL_ATTACK_UNITS)
{
}

void MillenniumFalcon::position(const Location& position, double speed)
{
    if (getFlightMode() == SpaceShip::FlightMode::DEAD)
    {
        throw std::logic_error("ERROR: Cannot set position for dead ship");
    }

    validateSpeed(speed);
    SpaceShip::setPosition(position);
    setSpeed(speed);
}

void MillenniumFalcon::destination(const std::weak_ptr<Station>& destination, double speed)
{
    if (getFlightMode() == SpaceShip::FlightMode::DEAD)
    {
        throw std::logic_error("ERROR: Cannot set destination for dead ship.");
    }

    validateSpeed(speed);
    SpaceShip::setDestination(destination);
    setSpeed(speed);
}

void MillenniumFalcon::course(double angle, double speed)
{
    if (getFlightMode() == SpaceShip::FlightMode::DEAD)
    {
        throw std::logic_error("ERROR: Cannot set course for dead ship.");
    }

    validateSpeed(speed);
    SpaceShip::setCourse(angle);
    setSpeed(speed);
}

void MillenniumFalcon::move()
{
    if (getFlightMode() != SpaceShip::FlightMode::DEAD)
    {
        SpaceShip::moveOneStep();
    }
}

void MillenniumFalcon::print(std::ostream& out) const
{
    SpaceShip::print(out);
    out << ", Strength: " << attackUnits;
}

bool MillenniumFalcon::attack(const std::weak_ptr<Shuttle>& shuttle)
{
    // make sure both attacker and attacke are not dead
    if (getFlightMode() == SpaceShip::FlightMode::DEAD)
    {
        throw std::logic_error("Error: Dead falcon cannot attack.");
    }
    if (shuttle.lock()->getFlightMode() == SpaceShip::FlightMode::DEAD)
    {
        throw std::logic_error("Error: Cannot attack a dead shuttle.");
    }

    bool attackSuccess = true;

    // attack failed due to distance / attack units
    if(shuttle.lock()->getLocation().distance(getLocation()) > MIN_ATTACK_DISTANCE_SHUTTLE
        || attackUnits <= shuttle.lock()->getDefenseUnit())
    {
        attackSuccess = false;
    }

    // attack failed due to presence of bomber
    for(const auto& pair:Model::getInstance().lock()->getShips())
    {     
        if(std::dynamic_pointer_cast<TieBomber>(pair.second)
            && pair.second->getLocation().distance(getLocation()) <= FAIL_ATTACK_DISTANCE_BOMBER)
        {
            // bomber in attack failure distance found
            attackSuccess = false;
            break;
        }
    }

    stop(); // stop in any case
    shuttle.lock()->getAttacked(attackSuccess); // attack shuttle in any case
    attackUnits += attackSuccess ? 1 : -1;
    
    if (attackUnits == 0)
    {
        die();
    }

    return attackSuccess;
}

std::string MillenniumFalcon::getType() const 
{
    return "falcon";
}
