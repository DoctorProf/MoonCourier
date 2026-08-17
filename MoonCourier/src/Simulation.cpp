#include "../include/Simulation.h"
#include "../include/RandomGenerator.h"

//Simulation::Simulation(json config)
//{
//    load(config);
//}

Simulation::Simulation(int width_grid, int height_grid)
    : map(width_grid, height_grid),
    base(
        RandomGenerator::get(0, width_grid - 1),
        RandomGenerator::get(0, height_grid - 1),
        RandomGenerator::get(100.0f, 500.0f)
    )
{
    map.setCellType(
        base.getX(),
        base.getY(),
        CellType::Regolith
    );

    for (int i = 0; i < 5; ++i)
    {
        int order_x = RandomGenerator::get(0, width_grid - 1);
        int order_y = RandomGenerator::get(0, height_grid - 1);

        map.setCellType(order_x, order_y, CellType::Regolith);

        orders.emplace_back(
            i,
            order_x,
            order_y,
            RandomGenerator::get(10.0f, 100.0f),
            RandomGenerator::get(10.0f, 50.0f),
            static_cast<OrderUrgency>(RandomGenerator::get(0, 3))
        );
    }
}

void Simulation::run()
{
    
}

void Simulation::update()
{
   
}

void Simulation::load(json config)
{
    
}

void Simulation::save()
{
    
}

Base& Simulation::getBase()
{
    return base;
}

Map& Simulation::getMap()
{
    return map;
}

std::vector<Order>& Simulation::getOrders()
{
    return orders;
}
void Simulation::addOrder(const Order& order)
{
    orders.push_back(order);
}
void Simulation::removeOrder(int orderId)
{
    orders.erase(orders.begin() + orderId);
}