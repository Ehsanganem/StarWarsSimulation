//
// Created by ganim on 15/04/2024.
//

#include "TieBomber.h"
#include "Commander.h"
#include <stdexcept>
#include "Model.h"

#define SPEED 1.0



void TieBomber::reset()
{
    visitedStationsNames.clear();
    patrolSrc.reset();
    currentDestination.reset();
}

TieBomber::TieBomber(const Location& location, const std::string& name,
    const std::weak_ptr<ImperialAgent>& imperialAgent) 
    : FreeMovementImperialAgentSpaceShip(location, name, imperialAgent, SPEED),
     patrolMode(TieBomber::PatrolMode::DONE), visitedStationsNames(), patrolSrc(),
    currentDestination()
{
    if (!std::dynamic_pointer_cast<Commander>(imperialAgent.lock())) {
        throw std::invalid_argument("pilot of TieBomber must be Commander");
    }
}

void TieBomber::position(const Location& position)
{
    FreeMovementImperialAgentSpaceShip::setPosition(position);
    patrolMode = PatrolMode::SINGLE_TARGET;
    reset();
}

void TieBomber::destination(const std::weak_ptr<Station>& destination)
{
    FreeMovementImperialAgentSpaceShip::setDestination(destination);
    patrolMode = PatrolMode::PATROLING;
    visitedStationsNames.clear();
    patrolSrc = destination;
    currentDestination = destination;
}

void TieBomber::course(double angle)
{
    FreeMovementImperialAgentSpaceShip::setCourse(angle);
    patrolMode = PatrolMode::SINGLE_TARGET;
    reset();
}


void TieBomber::move() 
{
  
    switch (patrolMode)
    {
    case TieBomber::PatrolMode::DONE:
        // do nothing on DONE
        break;
    case TieBomber::PatrolMode::SINGLE_TARGET:
        moveOneStep();

        if (getFlightMode() == SpaceShip::FlightMode::DOCKED)
        {
            patrolMode = TieBomber::PatrolMode::DONE;
        }
        break;

    case TieBomber::PatrolMode::PATROLING:
        moveOneStep();

        if (getFlightMode() == SpaceShip::FlightMode::DOCKED) // reached
        {
            visitedStationsNames.insert(currentDestination.lock()->getName());

            // look for nearest unvisited station
            std::weak_ptr<Station> nearestStation;
            double nearestDistance = -1.0;

            for (const auto& pair : Model::getInstance().lock()->getStations())
            {
                if (visitedStationsNames.count(pair.first) == 0) // unvisited
                {
                    double distance = pair.second->getLocation().distance(getLocation());

                    if (nearestStation.expired() || distance < nearestDistance)
                    {
                        nearestStation = pair.second;
                        nearestDistance = distance;
                    }
                }
            }

            if (!nearestStation.expired()) // found an additional station to visit
            {
                SpaceShip::setDestination(nearestStation);
                currentDestination = nearestStation;
            }
            else // additional station not found
            {
                setDestination(patrolSrc);
                currentDestination = patrolSrc;
                patrolMode = TieBomber::PatrolMode::ENDING_PATROL;
            }
        }
        break;

    case TieBomber::PatrolMode::ENDING_PATROL:
        moveOneStep();

        if (getFlightMode() == SpaceShip::FlightMode::DOCKED) // reached
        {
            reset();
            patrolMode = TieBomber::PatrolMode::DONE;
        }

        break;
    }
}

void TieBomber::stop()
{
    SpaceShip::stop();
    patrolMode = TieBomber::PatrolMode::DONE;
    reset();
}

std::string TieBomber::getType() const 
{
    return "bomber";
}


