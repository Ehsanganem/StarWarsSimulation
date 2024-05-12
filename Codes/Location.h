//
// Created by ganim on 15/04/2024.
//

#ifndef FINALPROJECT_LOCATION_H
#define FINALPROJECT_LOCATION_H


#include <string>
#include <ostream>

class Location {
private:
    double x;
    double y;
public:
    Location(double x,double y);
    static Location parse(const std::string& str);
    double getX()const;
    double getY()const;
    void setX(double x);
    void setY(double y);
    double distance(double x,double y) const;
    double distance(const Location& other) const;
    Location average(const Location& other) const;
    friend std::ostream & operator<<(std::ostream& out,const Location& loc);

};


#endif //FINALPROJECT_LOCATION_H
