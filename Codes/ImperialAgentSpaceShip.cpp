//
// Created by ganim on 15/04/2024.
//

#include "ImperialAgentSpaceShip.h"

ImperialAgentSpaceShip::ImperialAgentSpaceShip(const Location& location,
	const std::string& name, const std::weak_ptr<ImperialAgent>& imperialAgent, double speed)
	: SpaceShip(location, name, speed), imperialAgent(imperialAgent)
{
}

std::weak_ptr<ImperialAgent> ImperialAgentSpaceShip::getImperialAgent() const
{
	return imperialAgent;
}
