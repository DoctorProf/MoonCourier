#pragma once
#include <iostream>
#include <vector>
#include "../include/models/Order.h"
#include "../include/models/Base.h"
#include "../include/Map.h"
#include <nlohmann/json.hpp>
#include "Delivery.h"

using json = nlohmann::json;

class Simulation
{
public:
	Simulation(json config);
	Simulation(int width_grid, int height_grid);

	void update(float delta_time);

	void load(json config);
	void save();

	Base& getBase();
	Map& getMap();

	std::vector<Order>& getOrders();
	void addOrder(const Order& order);
	void removeOrder(int order_id);

	void createDelivery(
		Rover& rover,
		Order& order,
		const pathfinder::PathResult& path
	);

	std::vector<Delivery>& getDeliveries();

	void updateDeliveries();

private:
	Base base;
	Map map;
	std::vector<Order> orders;
	std::vector<Delivery> deliveries;
};