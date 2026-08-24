#pragma once
#include <iostream>
#include <vector>
#include "../include/models/Order.h"
#include "../include/models/Base.h"
#include "../include/Map.h"
#include "Delivery.h"


class Simulation
{
public:
	Simulation(int width_grid, int height_grid);

	void update(float delta_time);

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
	bool chargeRover(int rover_id);
	bool isGameOver() const;

	bool buyRover();

private:
	Base base;
	Map map;
	std::vector<Order> orders;
	std::vector<Delivery> deliveries;

	bool game_over = false;

	void generateOrder();
};