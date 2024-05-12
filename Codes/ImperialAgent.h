//
// Created by ganim on 15/04/2024.
//

#ifndef FINALPROJECT_IMPERIALAGENT_H
#define FINALPROJECT_IMPERIALAGENT_H
#include <string>


class ImperialAgent {
private:
    std::string name;
protected:
    ImperialAgent(const std::string& name); // protected so only subclasses can be instantiated
public:
    const std::string& getName() const;
    virtual ~ImperialAgent() = default;
};


#endif //FINALPROJECT_IMPERIALAGENT_H
