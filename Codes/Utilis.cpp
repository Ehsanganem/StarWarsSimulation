//
// Created by ganim on 24/04/2024.
//

#include "Utilis.h"
#include <cmath>
#include <sstream>
#define PI 3.1415926535

bool Utilis::doubleIsEqual(double x, double y, double epsilon) {
    return abs(x - y) <= epsilon;
}

double Utilis::clockwiseAngleToCounterClockwise(double clockwiseAngle)
{
    double baseAngle = -1 * clockwiseAngle + 90;
    return baseAngle >= 0.0 ? baseAngle : baseAngle + 360.0;
}

double Utilis::counterClockwiseAngleToClockwise(double counterClockwiseAngle)
{
    double baseAngle = -1 * counterClockwiseAngle + 90;
    return baseAngle >= 0.0 ? baseAngle : baseAngle + 360.0;
}

double Utilis::arctan(double dx, double dy) {
    // no angle
    if (doubleIsEqual(dx, 0.0) && doubleIsEqual(dy, 0.0))
    {
        return 0.0;
    }

    // special vertical case
    if (doubleIsEqual(dx, 0.0))
    {
        return dy > 0.0 ? 90.0 : 270.0;
    }

    double angel = atan(dy / dx) / PI * 180.0; // covert radian result to degrees

    if (dx < 0.0) // fix atan range
    {
        angel += 180.0;
    }
    if (angel < 0.0) // fix negative angle
    {
        angel += 360.0;
    }

    return angel;
}

double Utilis::sin(double degrees)
{
    // :: global namespace
    return ::sin((degrees / 180.0) * PI); // converted to radians
}

double Utilis::cos(double degrees)
{
    // :: global namespace
    return ::cos((degrees / 180.0) * PI); // converted to radians
}

// converts double to string in specified precision
std::string Utilis::to_string(double val, int precision)
{
    std::ostringstream stream;
    stream.precision(precision);
    stream << std::fixed << val;
    return stream.str();
}



