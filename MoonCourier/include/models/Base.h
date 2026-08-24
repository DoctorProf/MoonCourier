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
	bool chargeRover(int rover_id);

	void addMoney(float amount);
	float getMoney() const;
	void setMoney(float new_money);

	Rover* getSelectedRover();
	void selectNextRover();
	void selectPreviousRover();


private:
	int x;
	int y;
	std::vector<Rover> rovers;
	float money;
	int selected_rover_index = 0;
};