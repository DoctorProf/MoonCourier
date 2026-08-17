#pragma once

enum class OrderUrgency
{
    Low,
    Normal,
    High,
    Critical
};

class Order
{
public:
    Order(
        int id,
        int targetX,
        int targetY,
        float weight,
        float reward,
        OrderUrgency urgency
    );

    int getId() const;
    int getX() const;
    int getY() const;

    float getWeight() const;
    float getReward() const;
    OrderUrgency getUrgency() const;

private:
    int id;

    int x;
    int y;

    float weight;
    float reward;

    OrderUrgency urgency;
};