#include "View.h"
#include <string>
#include <vector>
#include <map>
#include "Model.h"
#include <algorithm>
#include <iostream>

#define DEFAULT_SIZE 25
#define MIN_SIZE 6
#define MAX_SIZE 30
#define DEFAULT_ZOOM 2.0
#define DEFAULT_PAN_X 0.0
#define DEFAULT_PAN_Y 0.0
#define EMPTY_SHOW_CELL ". "
#define GRADUATION_OFFSET 3

// static instance
std::shared_ptr<View> View::instance = nullptr;

View::View() : size(DEFAULT_SIZE), zoom(DEFAULT_ZOOM), pan(DEFAULT_PAN_X, DEFAULT_PAN_Y)
{

}

std::weak_ptr<Station> View::stationAtCell(int i, int j) const
{
    const std::map<std::string, std::shared_ptr<Station>>& stations
        = Model::getInstance().lock()->getStations();
    double y = yAtCell(i);
    double x = xAtCell(j);

    for (const auto& pair : stations)
    {
        if (pair.second->getLocation().getX() >= x && pair.second->getLocation().getX() < x + zoom
            && pair.second->getLocation().getY() >= y && pair.second->getLocation().getY() < y + zoom)
        {
            return pair.second;
        }
    }

    return std::weak_ptr<Station>();
}

std::weak_ptr<SpaceShip> View::spaceShipAtCell(int i, int j) const
{
    const std::map<std::string, std::shared_ptr<SpaceShip>>& spaceships
        = Model::getInstance().lock()->getShips();

    // look for space ships which appear at cell (i, j)
    // coordinates are translating into concrete location use pan + zoom factor
    double y = yAtCell(i);
    double x = xAtCell(j);

    for (const auto& pair : spaceships)
    {
        // ship coordinates are between current cell and next cell
        if (pair.second->getLocation().getX() >= x && pair.second->getLocation().getX() < x + zoom
            && pair.second->getLocation().getY() >= y && pair.second->getLocation().getY() < y + zoom)
        {
            return pair.second;
        }
    }

    return std::weak_ptr<SpaceShip>(); // no ship found
}

// calculate y coordinate of row i based on pan offset and zoom factor
double View::yAtCell(int i) const
{
    return pan.getY() + zoom * i;
}

// calculate x coordinate of row i based on pan offset and zoom factor
double View::xAtCell(int j) const
{
    return pan.getX() + zoom * j;
}

std::weak_ptr<View> View::getInstance()
{
	if (!instance)
	{
        // using new as std::make_shared requires ctor to be public
        // (made private for singleton impelmentation)
        instance = std::shared_ptr<View>(new View());
	}

	return instance;
}

void View::setDefault()
{
	size = DEFAULT_SIZE;
	zoom = DEFAULT_ZOOM;
	pan = Location(DEFAULT_PAN_X, DEFAULT_PAN_Y);
}

void View::setSize(int size)
{
    if (size < MIN_SIZE)
    {
        throw std::invalid_argument("ERROR: New map size is too small.");
    }
    if (size > MAX_SIZE)
    {
        throw std::invalid_argument("ERROR: New map size is too big.");
    }

    this->size = size;
}

void View::setZoom(double zoom)
{
    if (zoom < 0.0) 
    {
        throw std::invalid_argument("ERROR: New map scale must be positive");
    }

    this->zoom = zoom;
}

void View::setPan(const Location& pan)
{
    this->pan = pan;
}

void View::show() const
{
    // printing title
    std::cout << "Display size: " << size << ", scale: " << zoom
        << " , origin: " << pan << std::endl;

    std::vector<std::vector<std::string>> lines;

    // initializing empty map representation (top-down, left-right)
    // extra column for graduation (y values)
    for (int i = 0; i < size; i++)
    {
        lines.push_back(std::vector<std::string>());

        for (int j = 0; j < size + 1; j++)
        {
            lines[i].push_back(j == 0 ? std::string(GRADUATION_OFFSET, ' ') : EMPTY_SHOW_CELL);
        }
    }

    // setting spaceship representations at appropriate locations

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            std::weak_ptr<Station> station = stationAtCell(i, j);

            if (!station.expired()) // station found
            {
                lines[i][j + 1] = station.lock()->getName().substr(0, 2);
            }

            std::weak_ptr<SpaceShip> spaceShip = spaceShipAtCell(i, j);

            if (!spaceShip.expired()) // spaceship found
            {
                lines[i][j + 1] = spaceShip.lock()->getName().substr(0, 2);
            }
        }
    }

    // initialize graduation column (y values)
    for (int i = 0; i < size; i += GRADUATION_OFFSET)
    {
        // padding numbers with spaces
        int alignment = GRADUATION_OFFSET;
        std::string yStr = std::to_string((int)yAtCell(i));
        lines[i][0] = yStr + std::string(std::max(0, alignment - (int)yStr.length()), ' ');
    }

    // print lines (upside down)
    for (int i = (int)lines.size() - 1; i >= 0; i--)
    {
        for (int j = 0; j < (int)lines[i].size(); j++)
        {
            std::cout << lines[i][j];
        }

        std::cout << std::endl;
    }

    // print graduation row (x values)
    for (int j = 0; j < size; j += GRADUATION_OFFSET)
    {
        // padding numbers with spaces
        int alignment = j == 0 ? 4 : 2 * GRADUATION_OFFSET;
        std::string xStr = std::to_string((int)xAtCell(j));
        std::cout << std::string(std::max(0, alignment - (int)xStr.length()), ' ') + xStr;
    }

    std::cout << std::endl;
}