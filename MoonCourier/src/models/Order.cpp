#include "../../include/models/Order.h"

Order::Order(
    int id,
    int targetX,
    int targetY,
    float weight,
    float reward,
    OrderUrgency urgency
)
    : id(id),
    x(targetX),
    y(targetY),
    weight(weight),
    reward(reward),
    urgency(urgency)
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
