#include "../../include/models/Order.h"

Order::Order(
    int id,
    int target_x,
    int target_y,
    float weight,
    float reward,
    OrderUrgency urgency
)
    : id(id),
    x(target_x),
    y(target_y),
    weight(weight),
    reward(reward),
    urgency(urgency),
    status(OrderStatus::Pending)
{
}

int Order::getId() const
{
    return id;
}

int Order::getX() const
{
    return x;
}

int Order::getY() const
{
    return y;
}

float Order::getWeight() const
{
    return weight;
}

float Order::getReward() const
{
    return reward;
}

OrderUrgency Order::getUrgency() const
{
    return urgency;
}

OrderStatus Order::getStatus() const
{
    return status;
}

void Order::setStatus(OrderStatus new_status)
{
    status = new_status;
}
