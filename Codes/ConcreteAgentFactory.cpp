//
// Created by ganim on 15/04/2024.
//

#include "ConcreteAgentFactory.h"
#include "MidShipMan.h"
#include "Admiral.h"
#include <stdexcept>
#include "Commander.h"


std::shared_ptr<ImperialAgent> ConcreteAgentFactory::getAgent(AgentFactory::AgentType type,
    const std::string &name) {

    switch (type)
    {
        case AgentFactory::AgentType::MIDSHIPMAN:
            return std::make_shared<MidShipMan>(name);

        case AgentFactory::AgentType::ADMIRAL:
            return std::make_shared<Admiral>(name);

        case AgentFactory::AgentType::COMMANDER:
            return std::make_shared<Commander>(name);
    }

    return std::shared_ptr<ImperialAgent>();
}
