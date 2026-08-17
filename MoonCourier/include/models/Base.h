#pragma once
#include <vector>
#include "Rover.h"
#include "../RandomGenerator.h"

class Base
{
public:
	Base(int x, int y, float money = 200.f);
	int getX() const;

	int getY() const;
	std::vector<Rover>& getRovers();
	const std::vector<Rover>& getRovers() const;
	void addRover(const Rover& rover);
	void removeRover(int roverId);

	float getMoney() const;
	void setMoney(float newMoney);

	bool selectRover(int roverId);

	Rover* getSelectedRover();
	const Rover* getSelectedRover() const;

	bool chargeSelectedRover(float price);

private:
	int x;
	int y;
	std::vector<Rover> rovers;
	float money;
	int selected_rover_id = -1;
};