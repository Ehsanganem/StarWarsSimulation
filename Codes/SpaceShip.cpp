//
// Created by ganim on 15/04/2024.
//

#include "SpaceShip.h"
#include "Model.h"
#include <cmath>
#include "Utilis.h"

#define UNINITIALIZED_ANGLE -1.0

const std::string SpaceShip::flightModeStrings[] = {"stopped", "docked", "dead", "moving"};

SpaceShip::SpaceShip(const Location& location, const std::string& name, double speed)
    : location(location), name(name), speed(speed),
    position(), destination(), angle(UNINITIALIZED_ANGLE),
    flightMode(SpaceShip::FlightMode::STOPPED)
{
}

Location SpaceShip::getTargetLoocation()
{
    return position ? *position : destination.lock()->getLocation();
}

void SpaceShip::resetMovement()
{
    position.reset();
    destination.reset();
    angle = UNINITIALIZED_ANGLE;
}

void SpaceShip::moveOneStep() {
    if (flightMode != FlightMode::MOVING) {
        return;
    }

    double movementAngle;

    if (angle >= 0.0)
    {
        movementAngle = angle;
    }
    else
    {
        const Location& targetLocation = getTargetLoocation();

        // started at target
        if (Utilis::doubleIsEqual(targetLocation.distance(location), 0.0)) 
        {
            flightMode = position ? SpaceShip::FlightMode::STOPPED : SpaceShip::FlightMode::DOCKED;
            resetMovement();
            return;
        }

        movementAngle = Utilis::arctan(targetLocation.getX() - location.getX(),
            targetLocation.getY() - location.getY());
    }

    double vy = speed * Utilis::sin(movementAngle);
    double vx = speed * Utilis::cos(movementAngle);
    double newLocationX = location.getX() + vx;
    double newLocationY = location.getY() + vy;

    // update location
    location.setX(newLocationX);
    location.setY(newLocationY);

    // if movement is by angle, there is no target to reach

    // if movement is by position or destination
    if(position || !destination.expired())
    {
        const Location& targetLocation = getTargetLoocation();
        double movementAngleAfterMoving = Utilis::arctan(targetLocation.getX() - location.getX(),
            targetLocation.getY() - location.getY());

        // reached or passed target (new movement angle is opposite direction to 
        // current movement angle)
        if(Utilis::doubleIsEqual(targetLocation.distance(location), 0.0)
            || Utilis::doubleIsEqual(abs(movementAngle - movementAngleAfterMoving), 180.0))
        {
            location = targetLocation; // adjust location
            flightMode = position ? SpaceShip::FlightMode::STOPPED : SpaceShip::FlightMode::DOCKED;
            resetMovement();
        }
    }
}

void SpaceShip::stop()
{
    resetMovement();
    flightMode = SpaceShip::FlightMode::STOPPED;
}

void SpaceShip::die()
{
    resetMovement();
    flightMode = SpaceShip::FlightMode::DEAD;
}

void SpaceShip::setPosition(const Location& position)
{
    resetMovement();
    this->position = std::make_shared<Location>(position);
    flightMode = SpaceShip::FlightMode::MOVING;
}

void SpaceShip::setDestination(const std::weak_ptr<Station>& destination)
{
    resetMovement();
    this->destination = destination;
    flightMode = SpaceShip::FlightMode::MOVING;
}

void SpaceShip::setCourse(double angle)
{
    resetMovement();
    this->angle = angle;
    flightMode = SpaceShip::FlightMode::MOVING;
}

void SpaceShip::setSpeed(double speed)
{
    this->speed = speed;
}

const std::string& SpaceShip::getName() const
{
    return name;
}

const Location& SpaceShip::getLocation() const
{
    return location;
}

SpaceShip::FlightMode SpaceShip::getFlightMode() const
{
    return flightMode;
}

std::shared_ptr<Location> SpaceShip::getPosition() const
{
    return position;
}

std::weak_ptr<Station> SpaceShip::getDestination() const
{
    return destination;
}

double SpaceShip::getAngle() const
{
    return angle;
}

void SpaceShip::print(std::ostream& out) const
{
    out << getType() << " " << name << " at " << location << ",";

    if (getFlightMode() == SpaceShip::FlightMode::MOVING)
    {
        if (angle >= 0.0) // angle initialized
        {
            out << " Heading on course " << Utilis::counterClockwiseAngleToClockwise(angle)
                << " deg";
        }
        else // angle not initialized - movement by destination or position
        {
            out << " Heading to ";

            if (!destination.expired())
            {
                out << destination.lock()->getName();
            }
            else
            {
                out << *position;
            }
        }
    }
    else
    {
        out << " " << SpaceShip::flightModeStrings[(int)getFlightMode()];
    }


    out << ", speed " << (speed * Model::getInstance().lock()->getBaseScaleUnitKm())
        << "km/h";
}

std::ostream& operator<<(std::ostream& out, const SpaceShip& ship) {
    ship.print(out);
    return out;
}




