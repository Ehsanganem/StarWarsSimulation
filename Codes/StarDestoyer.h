//
// Created by ganim on 15/04/2024.
//

#ifndef FINALPROJECT_STARDESTOYER_H
#define FINALPROJECT_STARDESTOYER_H

#include <memory>
#include "SpaceShip.h"
#include "Location.h"
#include "FreeMovementImperialAgentSpaceShip.h"
#include <vector>

class StarDestoyer: public FreeMovementImperialAgentSpaceShip {
private:
    class Rocket : public FreeMovementImperialAgentSpaceShip
    {
    private:
        std::shared_ptr<Location> lastLocation;
    public:
        Rocket(const Location& location);
        virtual void move() override;
        std::string getType() const override;
    };

    std::vector<Rocket> rockets;

public:
    StarDestoyer(const Location& location, const std::string& name,
        const std::weak_ptr<ImperialAgent>& imperialAgent);
    void shoot(const Location& position);
    void move() override;
    std::string getType() const override;
};


#endif //FINALPROJECT_STARDESTOYER_H
