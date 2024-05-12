//
// Created by ganim on 15/04/2024.
//

#ifndef FINALPROJECT_SPACESHIP_H
#define FINALPROJECT_SPACESHIP_H

#include "Location.h"
#include "Station.h"
#include "ImperialAgent.h"
#include <string>
#include <memory>

class SpaceShip {
public:
    enum class FlightMode { STOPPED, DOCKED, DEAD, MOVING };
    static const std::string flightModeStrings[];
private:
    Location location;
    std::string name;
    double speed;
    std::shared_ptr<Location> position; // may be nullptr - no position
    std::weak_ptr<Station> destination; // may be nullptr - no destination
    double angle; // may be negative - no angle
    FlightMode flightMode;

    Location getTargetLoocation();
    void resetMovement();

protected:
    SpaceShip(const Location& location, const std::string& name, double speed);
    void moveOneStep();
    void setPosition(const Location& position);
    void setDestination(const std::weak_ptr<Station>& destination);
    void setCourse(double angle);
    virtual std::string getType() const = 0;

public:
    virtual ~SpaceShip() = default;

    // redefined due to default virtual dtor
    SpaceShip(const SpaceShip& other) = default;
    SpaceShip& operator=(const SpaceShip& other) = default;
    SpaceShip(SpaceShip&& other) = default;
    SpaceShip& operator=(SpaceShip&& other) = default;
    virtual void move() = 0;

    virtual void stop();
    virtual void die();
 
    void setSpeed(double speed);
    const std::string& getName() const;
    const Location& getLocation() const;
    FlightMode getFlightMode() const;
    std::shared_ptr<Location> getPosition() const;
    std::weak_ptr<Station> getDestination() const;
    double getAngle() const;
    virtual void print(std::ostream& out) const;
    
    friend std::ostream& operator<<(std::ostream& out, const SpaceShip& ship);
};


#endif //FINALPROJECT_SPACESHIP_H
