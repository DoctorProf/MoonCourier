#include "../include/Simulation.h"
#include "../include/RandomGenerator.h"

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
        int order_x =
            RandomGenerator::get(0, width_grid - 1);

        int order_y =
            RandomGenerator::get(0, height_grid - 1);

        if (order_x == base.getX() &&
            order_y == base.getY())
        {
            --i;
            continue;
        }

        map.setCellType(
            order_x,
            order_y,
            CellType::Regolith
        );

        orders.emplace_back(
            RandomGenerator::get(0, 10000),
            order_x,
            order_y,
            RandomGenerator::get(20.0f, 200.0f),
            RandomGenerator::get(50.0f, 150.0f),
            static_cast<OrderUrgency>(
                RandomGenerator::get(0, 3)
                )
        );
    }
}

void Simulation::update(float delta_time)
{
    for (auto& rover : base.getRovers())
    {
        if (!rover.isMoving())
            continue;

        rover.updateMovement(
            delta_time,
            map.getCells(),
            map.getWidthGrid()
        );
    }

    std::vector<int> dead_rovers;

    for (const auto& rover : base.getRovers())
    {
        if (rover.isDead())
        {
            dead_rovers.push_back(
                rover.getId()
            );
        }
    }

    for (int rover_id : dead_rovers)
    {
        deliveries.erase(
            std::remove_if(
                deliveries.begin(),
                deliveries.end(),
                [rover_id](const Delivery& delivery)
                {
                    return delivery.rover_id == rover_id;
                }
            ),
            deliveries.end()
        );

        base.removeRover(rover_id);
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
)
{
    deliveries.emplace_back(
        rover.getId(),
        order.getId(),
        order.getReward(),
        path,
        DeliveryState::ToOrder
    );

    float load_multiplier =
        1.0f + (order.getWeight() / 100.0f) * 0.2f;

    rover.setLoadMultiplier(load_multiplier);

    rover.setPath(path.cells);

    rover.setPathDirection(
        PathDirection::Forward
    );

    rover.setMoving(true);
}

std::vector<Delivery>& Simulation::getDeliveries()
{
    return deliveries;
}

void Simulation::updateDeliveries()
{
    for (auto it = deliveries.begin();
        it != deliveries.end();)
    {
        Delivery& delivery = *it;

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
        {
            it = deliveries.erase(it);
            continue;
        }

        if (rover->isMoving())
        {
            ++it;
            continue;
        }

        if (delivery.state == DeliveryState::ToOrder)
        {
            auto order_it = std::find_if(
                orders.begin(),
                orders.end(),
                [&delivery](const Order& order)
                {
                    return order.getId() ==
                        delivery.order_id;
                }
            );

            if (order_it != orders.end())
            {
                order_it->setStatus(
                    OrderStatus::Delivered
                );

                orders.erase(order_it);
            }

            generateOrder();

            delivery.state =
                DeliveryState::ToBase;

            rover->setPathDirection(
                PathDirection::Backward
            );

            rover->setMoving(true);

            ++it;
            continue;
        }

        if (delivery.state == DeliveryState::ToBase)
        {
            base.addMoney(
                delivery.reward
            );

            rover->setLoadMultiplier(1.0f);
            rover->setMoving(false);

            it = deliveries.erase(it);

            continue;
        }

        ++it;
    }
}

void Simulation::generateOrder()
{
    int width =
        map.getWidthGrid();

    int height =
        map.getHeightGrid();

    while (true)
    {
        int order_x =
            RandomGenerator::get(
                0,
                width - 1
            );

        int order_y =
            RandomGenerator::get(
                0,
                height - 1
            );

        if (order_x == base.getX() &&
            order_y == base.getY())
        {
            continue;
        }

        const Cell& cell =
            map.getCells()[
                order_y * width + order_x
            ];

        if (!cell.isPassable())
            continue;

        orders.emplace_back(
            RandomGenerator::get(0, 10000),
            order_x,
            order_y,
            RandomGenerator::get(20.0f, 200.0f),
            RandomGenerator::get(50.0f, 150.0f),
            static_cast<OrderUrgency>(
                RandomGenerator::get(0, 3)
                )
        );

        break;
    }
}

bool Simulation::chargeRover(int rover_id)
{
    if (game_over)
        return false;

    Rover* rover = nullptr;

    for (auto& candidate : base.getRovers())
    {
        if (candidate.getId() == rover_id)
        {
            rover = &candidate;
            break;
        }
    }

    if (!rover)
        return false;

    if (rover->isMoving())
        return false;

    if (rover->getX() != base.getX() ||
        rover->getY() != base.getY())
    {
        return false;
    }

    if (rover->getBatteryLevel() >= 100.0f)
        return true;

    float charge_cost =
        100.0f - rover->getBatteryLevel();

    if (base.getMoney() < charge_cost)
    {
        game_over = true;

        std::cout << "You lost!\n";
        std::cout << "Game Over.\n";

        return false;
    }

    base.setMoney(
        base.getMoney() - charge_cost
    );

    rover->setBatteryLevel(100.0f);

    return true;
}

bool Simulation::buyRover()
{
    constexpr float rover_price = 500.0f;

    if (game_over)
        return false;

    if (!base.getRovers().empty())
        return false;

    if (base.getMoney() < rover_price)
    {
        game_over = true;

        std::cout << "You lost!\n";
        std::cout << "Game Over.\n";

        return false;
    }

    base.setMoney(
        base.getMoney() - rover_price
    );

    int rover_id =
        RandomGenerator::get(
            0,
            10000
        );

    base.addRover(
        Rover(
            base.getX(),
            base.getY(),
            rover_id,
            false,
            100.0f,
            0.0f
        )
    );

    return true;
}

bool Simulation::isGameOver() const
{
    return game_over;
}