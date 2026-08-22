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
	Rover& getRover(int rover_id);
	void addRover(const Rover& rover);
	void removeRover(int rover_id);
	void chargeRover(int rover_id);

	float getMoney() const;
	void setMoney(float newMoney);


private:
	int x;
	int y;
	std::vector<Rover> rovers;
	float money;
};