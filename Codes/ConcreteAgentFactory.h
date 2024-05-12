//
// Created by ganim on 15/04/2024.
//

#ifndef FINALPROJECT_CONCRETEFACTORY_H
#define FINALPROJECT_CONCRETEFACTORY_H
#include "AgentFactory.h"

class ConcreteAgentFactory : public AgentFactory {
    std::shared_ptr<ImperialAgent> getAgent(AgentFactory::AgentType type,
        const std::string& name) override;
};


#endif //FINALPROJECT_CONCRETEFACTORY_H
