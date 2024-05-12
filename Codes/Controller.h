//
// Created by ganim on 28/04/2024.
//

#ifndef FINALPROJECT_CONTROLLER_H
#define FINALPROJECT_CONTROLLER_H

#include <memory>
#include <sstream>
#include "Model.h"
#include <string>


class Controller {
private:
    enum class eCommand
    {
        DEFAULT, SIZE, ZOOM, PAN, SHOW, STATUS, GO, CREATE, EXIT, COURSE,
        POSITION, DESTINATION, ATTACK, SHOOT, STOP, START_SUPPLY
    };

    static std::string commandStrings[];

    void handleShipCommand(const std::string& shipName,
        const std::weak_ptr<Model>& model,
        std::istringstream& stream);
    void handleCreateCommand(const std::weak_ptr<Model>& model,
        std::istringstream& stream);

public:
    void run(int argc, char* argv[]);
};


#endif //FINALPROJECT_CONTROLLER_H
