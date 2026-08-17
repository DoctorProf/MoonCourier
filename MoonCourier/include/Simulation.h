#pragma once
#include <iostream>
#include <vector>
#include "../include/models/Order.h"
#include "../include/models/Base.h"
#include "../include/Map.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Simulation
{
public:
	Simulation(json config);
	Simulation(int widthGrid, int heightGrid);

	void run();
	void update();

	void load(json config);
	void save();

	Base& getBase();
	Map& getMap();

	std::vector<Order>& getOrders();
	void addOrder(const Order& order);
	void removeOrder(int orderId);

private:
	Base base;
	Map map;
	std::vector<Order> orders;
};