//
// Created by ganim on 15/04/2024.
//

#ifndef FINALPROJECT_IMPERIALAGENTSPACESHIP_H
#define FINALPROJECT_IMPERIALAGENTSPACESHIP_H

#include "SpaceShip.h"
#include <memory>
#include "Location.h"
#include <string>
#include "ImperialAgent.h"

// represents a spaceship piloted by an imperial agent
class ImperialAgentSpaceShip : public SpaceShip
{
private:
	std::weak_ptr<ImperialAgent> imperialAgent;

public:
	ImperialAgentSpaceShip(const Location& location, const std::string& name,
		const std::weak_ptr<ImperialAgent>& imperialAgent, double speed);

	std::weak_ptr<ImperialAgent> getImperialAgent() const;
};

#endif // FINALPROJECT_IMPERIALAGENTSPACESHIP_H