#pragma once
#include <iostream>
#include <vector>
#include "../include/objects/Order.h"
#include "../include/objects/Rover.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Simulation
{
public:
	Simulation(json config);
	Simulation(int base_x, int base_y, int width_grid, int height_grid, int size_cell, int money);
	void run();
	void update();
	void load(json config);
	void save();
	int getBaseX() const;
	int getBaseY() const;
	int getWidthGrid() const;
	int getHeightGrid() const;
	int getSizeCell() const;
	int getMoney() const;

	std::vector<Order>& getOrders();
	std::vector<Rover>& getRovers();

	void addOrder(const Order& order);
	void addRover(const Rover& rover);

private:
	int base_x;
	int base_y;
	int width_grid;
	int height_grid;
	int size_cell;
	int money;
	std::vector<Order> orders;
	std::vector<Rover> rovers;
	
};