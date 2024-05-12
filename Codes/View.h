//
// Created by ganim on 28/04/2024.
//


#ifndef FINALPROJECT_VIEW_H
#define FINALPROJECT_VIEW_H

#include <memory>
#include "Location.h"
#include "SpaceShip.h"
class View
{
private:
	static std::shared_ptr<View> instance;
	int size;
	double zoom;
	Location pan;

	View(); // private ctor for singleton

	std::weak_ptr<Station> stationAtCell(int i, int j) const;
	std::weak_ptr<SpaceShip> spaceShipAtCell(int i, int j) const;
	double yAtCell(int i) const;
	double xAtCell(int j) const;
public:
	static std::weak_ptr<View> getInstance();
	void setZoom(double zoom);
	void setSize(int size);
	void setPan(const Location& pan);
	void setDefault();
	void show() const;
};

#endif //FINALPROJECT_VIEW_H
