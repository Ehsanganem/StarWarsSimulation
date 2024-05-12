#include "FreeMovementImperialAgentSpaceShip.h"

FreeMovementImperialAgentSpaceShip::FreeMovementImperialAgentSpaceShip(const Location& location, 
	const std::string& name, const std::weak_ptr<ImperialAgent>& imperialAgent, double speed)
	: ImperialAgentSpaceShip(location, name, imperialAgent, speed)
{
}

void FreeMovementImperialAgentSpaceShip::position(const Location& position)
{
	if (getFlightMode() == SpaceShip::FlightMode::DEAD)
	{
		throw std::logic_error("Cannot set position for dead ship");
	}

	ImperialAgentSpaceShip::setPosition(position);
}

void FreeMovementImperialAgentSpaceShip::destination(const std::weak_ptr<Station>& destination)
{
	if (getFlightMode() == SpaceShip::FlightMode::DEAD)
	{
		throw std::logic_error("Cannot set position for dead ship");
	}

	ImperialAgentSpaceShip::setDestination(destination);
}

void FreeMovementImperialAgentSpaceShip::course(double angle)
{
	if (getFlightMode() == SpaceShip::FlightMode::DEAD)
	{
		throw std::logic_error("Cannot set position for dead ship");
	}

	ImperialAgentSpaceShip::setCourse(angle);
}
