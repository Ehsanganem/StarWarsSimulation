//
// Created by ganim on 28/04/2024.
//

#include "Controller.h"
#include <memory>
#include <iostream>
#include "Model.h"
#include "Utilis.h"
#include "View.h"
#include <sstream>
#include "MillenniumFalcon.h"

#define NUM_ARGC 2


std::string Controller::commandStrings[] =
{
    "default", "size", "zoom", "pan", "show", "status", "go", "create", "exit", "course",
    "position", "destination", "attack", "shoot", "stop", "start_supply"
};


void Controller::run(int argc, char* argv[])
{
    if (argc != NUM_ARGC)
    {
        std::cout << "Wrong number of arguments, required: <site_file_name>" << std::endl;
        return;
    }

    std::weak_ptr<Model> model;

    try
    {
        model = Model::getInstance(argv[1]);
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what() << std::endl;
        return;
    }
   

    std::string line;
    bool keepRunning = true;

    while (keepRunning) 
    {
        std::cout << "Time" << " " << model.lock()->getTime() << ": Enter command: ";
        std::getline(std::cin, line);
        std::istringstream stream(line);
        std::string command;

        stream >> command;

        if (command == commandStrings[(int)Controller::eCommand::DEFAULT])
        {
            View::getInstance().lock()->setDefault();
        }
        else if (command == commandStrings[(int)Controller::eCommand::SIZE])
        {
            int size;
            stream >> size;

            if (!stream) // failed reading an integer
            {
                std::cout << "ERROR: Expected an integer." << std::endl;
            }
            else
            {
                try
                {
                    View::getInstance().lock()->setSize(size);
                }
                catch (const std::invalid_argument& e)
                {
                    std::cout << e.what() << std::endl;
                }
            }
        }
        else if (command == commandStrings[(int)Controller::eCommand::ZOOM])
        {
            double zoom;
            stream >> zoom;

            if (!stream) // failed reading a double
            {
                std::cout << "ERROR: Expected a double." << std::endl;
            }
            else
            {
                try
                {
                    View::getInstance().lock()->setZoom(zoom);
                }
                catch (const std::invalid_argument& e)
                {
                    std::cout << e.what() << std::endl;
                }
            }
        }
        else if (command == commandStrings[(int)Controller::eCommand::PAN])
        {
            double x, y;
            stream >> x >> y;

            if (!stream) // failed reading two doubles
            {
                std::cout << "ERROR: Expected two doubles." << std::endl;
            }
            else
            {
                Location location(x, y);
                View::getInstance().lock()->setPan(location);
            }
        }
        else if (command == commandStrings[(int)Controller::eCommand::SHOW])
        {
            View::getInstance().lock()->show();
        }
        else if (command == commandStrings[(int)Controller::eCommand::STATUS])
        {
            model.lock()->status();
        }
        else if (command == commandStrings[(int)Controller::eCommand::GO])
        {
            model.lock()->go();
        }
        else if (command == commandStrings[(int)Controller::eCommand::CREATE])
        {
            handleCreateCommand(model, stream);
        }
        else if (command == commandStrings[(int)Controller::eCommand::EXIT])
        {
            keepRunning = false;
        }
        else 
        {
            handleShipCommand(command, model, stream);
        }
    }
}


void Controller::handleShipCommand(const std::string& shipName,
    const std::weak_ptr<Model>& model,
    std::istringstream& stream)
{

    if (model.lock()->getShips().count(shipName) == 0) // ship name not found
    {
        std::cout << "ERROR: invalid command" << std::endl;
        return;
    }

    std::string shipCommand;
    stream >> shipCommand;

    try 
    {
        if (shipCommand == commandStrings[(int)Controller::eCommand::COURSE])
        {
            double angle;
       
            stream >> angle;

            angle = Utilis::clockwiseAngleToCounterClockwise(angle);

            if (std::dynamic_pointer_cast<MillenniumFalcon>(model.lock()->getShips().at(shipName)))
            {
                int speed;
                stream >> speed;

                if (!stream) // invalid speed
                {
                    std::cout << "ERROR: Expected an integer." << std::endl;
                }
                else
                {
                    model.lock()->setCourse(shipName, angle,
                        (double)speed / model.lock()->getBaseScaleUnitKm());
                }
            }
            else
            {
                model.lock()->setCourse(shipName, angle);
            }  
        }
        else if (shipCommand == commandStrings[(int)Controller::eCommand::POSITION])
        {
            std::string locationString;
          
            std::getline(stream, locationString);

            if (std::dynamic_pointer_cast<MillenniumFalcon>(model.lock()->getShips().at(shipName)))
            {
                    // ship is a falcon
                    // extract speed appearing after space, right after location coordinates
                    int speed;
                    size_t speedBeginIndex = locationString.find_last_of(' ') + 1;     

                    try
                    {
                        speed = stoi(locationString.substr(speedBeginIndex));

                    }
                    catch (const std::invalid_argument&)
                    {
                        std::cout << "ERROR: Expected an integer." << std::endl;
                        speed = -1;
                    }

                    if (speed != -1) // valid speed entered
                    {
                        model.lock()->setPosition(shipName,
                            Location::parse(locationString.substr(0, speedBeginIndex - 1)),
                            (double)speed / model.lock()->getBaseScaleUnitKm());
                    }        
            }
            else
            {
                model.lock()->setPosition(shipName, Location::parse(locationString));
            }    
        }
        else if (shipCommand == commandStrings[(int)Controller::eCommand::DESTINATION])
        {
            std::string siteName;
            stream >> siteName;

            if (std::dynamic_pointer_cast<MillenniumFalcon>(model.lock()->getShips().at(shipName)))
            {
                // ship is a falcon
                int speed;
                stream >> speed;

                if (!stream)
                {
                    std::cout << "ERROR: Expected an integer." << std::endl;
                }
                else
                {
                    model.lock()->setDestination(shipName, siteName, \
                        (double)speed / model.lock()->getBaseScaleUnitKm());
                }
            }
            else
            {
                model.lock()->setDestination(shipName, siteName);
            }
        }
        else if (shipCommand == commandStrings[(int)Controller::eCommand::DEFAULT])
        {
            std::string stationName;
            stream >> stationName;

            if (std::dynamic_pointer_cast<MillenniumFalcon>(model.lock()->getShips().at(shipName)))
            {
                // ship is a falcon
                int speed;
                stream >> speed;

                if (!stream) // not a valid integer
                {
                    std::cout << "ERROR: Expected an integer." << std::endl;
                }
                else
                {
                    model.lock()->setDestination(shipName, stationName, 
                        (double)speed / model.lock()->getBaseScaleUnitKm());
                }
            }
            else
            {
                model.lock()->setDestination(shipName, stationName);
            }
        }
        else if (shipCommand == commandStrings[(int)Controller::eCommand::ATTACK])
        {
            std::string attackedSship;
            stream >> attackedSship;
            model.lock()->attack(shipName, attackedSship);
        }
        else if (shipCommand == commandStrings[(int)Controller::eCommand::SHOOT])
        {
            std::string locationString;
            std::getline(stream, locationString);
            model.lock()->shoot(shipName, Location::parse(locationString));
        }
        else if (shipCommand == commandStrings[(int)Controller::eCommand::STOP])
        {
            model.lock()->stop(shipName);
        }
        else if (shipCommand == commandStrings[(int)Controller::eCommand::START_SUPPLY])
        {
            std::string sourceStationName;
            std::string destinationFortressStarName;

            stream >> sourceStationName >> destinationFortressStarName;

            if (!stream) // source and destination names not read correctly
            {
                std::cout 
                    << "ERROR: Expected an source station name and destination fortress start name." 
                    << std::endl;
            }

            model.lock()->startSupply(shipName, sourceStationName, destinationFortressStarName);
        }
        else
        {
            std::cout << "ERROR: invalid ship command." << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Controller::handleCreateCommand(const std::weak_ptr<Model>& model,
    std::istringstream& stream)
{
    std::string type;
    stream >> type;

    if (Model::agentTypeMap.count(type) > 0) // creating agent
    {
        std::string agentName;
        stream >> agentName;

        try
        {
            model.lock()->createAgent(agentName, Model::agentTypeMap.at(type));
        }
        catch (const std::invalid_argument& e) 
        {
            std::cout << e.what() << std::endl;
        }
    }
    else if(Model::shipTypeMap.count(type) > 0) // creating ship
    {
        std::string shipName, pilotName;
        std::string startPosition;
        
        stream >> shipName;

        if (Model::shipTypeMap.at(type) != SpaceShipFactory::SpaceshipType::FALCON)
        {
            // not a falcon - read pilot name
            stream >> pilotName;
        }

        std::getline(stream, startPosition);

        try 
        {
            if (startPosition[1] == '(') // by location
            {
                model.lock()->createSpaceship(model.lock()->shipTypeMap.at(type),
                    shipName,
                    Location::parse(startPosition),
                    pilotName);
            }
            else // by station name 
            {
                if (model.lock()->getStations().count(startPosition) > 0)
                {
                    model.lock()->createSpaceship(model.lock()->shipTypeMap.at(type),
                        shipName,
                        model.lock()->getStations().at(startPosition)->getLocation(),
                        pilotName);
                }
                else 
                {
                    std::cout << "ERROR: invalid station name";
                }
            }
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << e.what() << std::endl;
        }    
    }
    else
    {
        std::cout << "ERROR: Invalid create command" << std::endl;
    }
}
