//
// Created by ganim on 15/04/2024.
//

#include "Location.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <algorithm>

Location::Location(double x, double y) :x(x), y(y) {

}

double Location::getX() const {
    return this->x;
}

double Location::getY() const {
    return this->y;
}

void Location::setX(double x) {
    this->x = x;

}

void Location::setY(double y) {
    this->y = y;

}

double Location::distance(double x, double y) const {
    double dx = x - this->x;
    double dy = y - this->y;
    return sqrt(dx * dx + dy * dy);
}

double Location::distance(const Location& other)const {
    return distance(other.x, other.y);
}

// returns average location between this and other
Location Location::average(const Location& other) const
{
    return Location((x + other.x) / 2.0, (y + other.y) / 2.0);
}

Location Location::parse(const std::string& str) {
    double x, y;
    std::string copy = str;

    // removes delimiters and wrappers: , ( )
    copy.erase(std::remove(copy.begin(), copy.end(), ','), copy.end());
    copy.erase(std::remove(copy.begin(), copy.end(), '('), copy.end());
    copy.erase(std::remove(copy.begin(), copy.end(), ')'), copy.end());

    std::stringstream stream(copy);
    stream >> x >> y ;

    if (!stream)
    {
        throw std::invalid_argument("Error: invalid location format.");
    }

    return Location(x, y);
}

std::ostream& operator<<(std::ostream& out, const Location& loc) {
    return out << "(" << loc.x << ", " << loc.y << ")";
}