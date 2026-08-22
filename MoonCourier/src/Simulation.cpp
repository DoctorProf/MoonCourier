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
        RandomGenerator::get(100.0f, 300.0f)
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
            RandomGenerator::get(0, 10000),
            order_x,
            order_y,
            RandomGenerator::get(10.0f, 100.0f),
            RandomGenerator::get(50.0f, 100.0f),
            static_cast<OrderUrgency>(RandomGenerator::get(0, 3))
        );
    }
}

void Simulation::update(float delta_time)
{
    for (auto& rover : base.getRovers())
    {
        if (!rover.getActive())
            continue;

        rover.updateMovement(
            delta_time,
            map.getCells(),
            map.getWidthGrid()
        );
    }

    updateDeliveries();
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
void Simulation::removeOrder(int order_id)
{
    auto it = std::find_if(
        orders.begin(),
        orders.end(),
        [order_id](const Order& order)
        {
            return order.getId() == order_id;
        }
    );

    if (it == orders.end())
        return;
    orders.erase(it);
}

void Simulation::createDelivery(
    Rover& rover,
    Order& order,
    const pathfinder::PathResult& path
) {
    deliveries.emplace_back(
        rover.getId(),
        order.getId(),
        path,
        DeliveryState::ToOrder
    );
    float load_multiplier =
        1.0f + (order.getWeight() / 100.0f) * 0.2f;

    rover.setLoadMultiplier(load_multiplier);
    rover.setPath(path.cells);
    rover.setMoving(true);
}

std::vector<Delivery>& Simulation::getDeliveries() {
	return deliveries;
}

void Simulation::updateDeliveries()
{
    for (auto& delivery : deliveries)
    {
        Rover* rover = nullptr;

        for (auto& candidate : base.getRovers())
        {
            if (candidate.getId() == delivery.rover_id)
            {
                rover = &candidate;
                break;
            }
        }

        if (!rover)
            continue;

        if (rover->isMoving())
            continue;

        if (delivery.state == DeliveryState::ToOrder)
        {
            delivery.state = DeliveryState::ToBase;

            rover->setPathDirection(
                PathDirection::Backward
            );

            rover->setMoving(true);
        }
    }
}