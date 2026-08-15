#include "../include/Simulation.h"
#include "../include/RandomGenerator.h"

Simulation::Simulation(json config)
{
    load(config);
	for (auto& order : config["orders"])
	{
		Order newOrder(
			order.value("id", 0),
			order.value("position.x", 0),
			order.value("position.y", 0),
			order.value("weight", 0.f),
			order.value("reward", 0.f),
			static_cast<OrderUrgency>(order.value("urgency", 0))
		);
		orders.push_back(newOrder);
	}
	for (auto& rover : config["rovers"])
	{
		Rover newRover(
			rover.value("position.x", 0),
			rover.value("position.y", 0),
			rover.value("id", 0),
			rover.value("active", false),
			rover.value("battery_life", 100.f),
			rover.value("load_capacity", 0.f)
		);
		rovers.push_back(newRover);
	}
}

Simulation::Simulation(int base_x, int base_y, int width_grid, int height_grid, int size_cell, int money)
    : base_x(base_x), base_y(base_y), width_grid(width_grid), height_grid(height_grid), size_cell(size_cell), money(money)
{
	for (int i = 0; i < 5; ++i)
	{
		Order new_order(
			i,
			RandomGenerator::get(0, width_grid - 1),
			RandomGenerator::get(0, height_grid - 1),
			RandomGenerator::get(1.f, 10.f),
			RandomGenerator::get(10.f, 100.f),
			static_cast<OrderUrgency>(RandomGenerator::get(0, 3))
		);
		orders.push_back(new_order);
	}
	Rover new_rover(
		base_x,
		base_y,
		0,
		true,
		RandomGenerator::get(50.f, 100.f),
		RandomGenerator::get(5.f, 20.f)
	);
	rovers.push_back(new_rover);
}

void Simulation::run()
{
    
}

void Simulation::update()
{
   
}

void Simulation::load(json config)
{
    base_x = config.value("base.position.x", 10);
    base_y = config.value("base.position.y", 10);
    width_grid = config.value("width_grid", 64);
    height_grid = config.value("height_grid", 36);
    size_cell = config.value("size_cell", 20);
    money = config.value("money", 1000);
}

void Simulation::save()
{
    // Implementation for saving the simulation
}

int Simulation::getBaseX() const
{
    return base_x;
}

int Simulation::getBaseY() const
{
    return base_y;
}

int Simulation::getWidthGrid() const
{
    return width_grid;
}

int Simulation::getHeightGrid() const
{
    return height_grid;
}

int Simulation::getSizeCell() const
{
    return size_cell;
}

int Simulation::getMoney() const
{
    return money;
}

std::vector<Order>& Simulation::getOrders()
{
    return orders;
}

std::vector<Rover>& Simulation::getRovers()
{
    return rovers;
}

void Simulation::addOrder(const Order& order)
{
    orders.push_back(order);
}

void Simulation::addRover(const Rover& rover)
{
    rovers.push_back(rover);
}