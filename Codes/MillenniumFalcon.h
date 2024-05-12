//
// Created by ganim on 15/04/2024.
//

#ifndef FINALPROJECT_MILLENNIUMFALCON_H
#define FINALPROJECT_MILLENNIUMFALCON_H


#include <string>
#include "SpaceShip.h"
#include "Shuttle.h"

#define MAX_SPEED 3.0

class MillenniumFalcon: public SpaceShip {
private:
    int attackUnits;

    static void validateSpeed(double speed);

public:
    MillenniumFalcon(const Location& location, const std::string& name, double speed = MAX_SPEED);
    void position(const Location& position, double speed);
    void destination(const std::weak_ptr<Station>& destination, double speed);
    void course(double angle, double speed);
    void move() override;
    void print(std::ostream& out) const override;
    bool attack(const std::weak_ptr<Shuttle>& shuttle);
    std::string getType()const override;

};


#endif //FINALPROJECT_MILLENNIUMFALCON_H
