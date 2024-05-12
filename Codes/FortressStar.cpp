//
// Created by ganim on 15/04/2024.
//

#include "FortressStar.h"

#define PRODUCTION_RATE 0

FortressStar::FortressStar(const std::string& name, const Location& location, int amount)
    : Station(name, location, amount, PRODUCTION_RATE) {

}

void FortressStar::loadContainer(int loadAmount) {
    this->amount += loadAmount;

}

std::string FortressStar::getType() const {
    return "fortress";
}

