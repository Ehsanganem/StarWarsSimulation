//
// Created by ganim on 15/04/2024.
//

#ifndef FINALPROJECT_TIEBOMBER_H
#define FINALPROJECT_TIEBOMBER_H

#include "SpaceShip.h"
#include "FreeMovementImperialAgentSpaceShip.h"
#include "Station.h"
#include <memory>
#include <string>
#include <set>

class TieBomber: public FreeMovementImperialAgentSpaceShip {
private:
    enum class PatrolMode {PATROLING, ENDING_PATROL, SINGLE_TARGET, DONE};
    PatrolMode patrolMode;
    std::set<std::string> visitedStationsNames;
    std::weak_ptr<Station> patrolSrc;
    std::weak_ptr<Station> currentDestination;

    void reset();
public:
    TieBomber(const Location& location, const std::string& name,
        const std::weak_ptr<ImperialAgent>& imperialAgent);
    void position(const Location& position) override;
    void destination(const std::weak_ptr<Station>& destination) override;
    void course(double angle) override;
    void move() override;
    void stop() override;
    std::string getType() const override;

};


#endif //FINALPROJECT_TIEBOMBER_H
