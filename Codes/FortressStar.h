//
// Created by ganim on 15/04/2024.
//

#ifndef FINALPROJECT_FORTRESSSTAR_H
#define FINALPROJECT_FORTRESSSTAR_H

#include "Station.h"


class FortressStar : public Station {
public:
    FortressStar(const std::string& name, const Location& location, int amount);
    void loadContainer(int loadAmount);
    std::string getType() const override;
};


#endif //FINALPROJECT_FORTRESSSTAR_H
