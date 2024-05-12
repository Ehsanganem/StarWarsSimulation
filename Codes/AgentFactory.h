//
// Created by ganim on 15/04/2024.
//

#ifndef FINALPROJECT_AGENTFACTORY_H
#define FINALPROJECT_AGENTFACTORY_H

#include <string>
#include <memory>
#include <map>
#include "ImperialAgent.h"

class AgentFactory {

public:
    enum class AgentType { MIDSHIPMAN, ADMIRAL, COMMANDER };
    virtual std::shared_ptr<ImperialAgent> getAgent(AgentType type, const std::string& name) = 0;
};


#endif //FINALPROJECT_AGENTFACTORY_H
