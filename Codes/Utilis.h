//
// Created by ganim on 24/04/2024.
//

#ifndef FINALPROJECT_UTILIS_H
#define FINALPROJECT_UTILIS_H
#include "Location.h"
#include <string>

#define DEFALUT_EPSILON 0.000001

// static utility class
class Utilis {
private:
    Utilis() = default; // ctor made private so class cannot be instantiated

public:
    static bool doubleIsEqual(double x, double y, double epsilon = DEFALUT_EPSILON);
    static double clockwiseAngleToCounterClockwise(double clockwiseAngle);
    static double counterClockwiseAngleToClockwise(double counterClockwiseAngle);

    // returns degrees in range [0-360)
    static double arctan(double dx, double dy);
    static double sin(double degrees);
    static double cos(double degrees);
    static std::string to_string(double val, int precision);
};


#endif //FINALPROJECT_UTILIS_H
