#pragma once

enum class OrderStatus
{
    Pending,
    InProgress,
    Delivered
};

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
        int target_x,
        int target_y,
        float weight,
        float reward,
        OrderUrgency urgency,
		OrderStatus status
    );

    int getId() const;
    int getX() const;
    int getY() const;

    float getWeight() const;
    float getReward() const;
    OrderUrgency getUrgency() const;

    OrderStatus getStatus() const;
    void setStatus(OrderStatus newStatus);

private:
    int id;

    int x;
    int y;

    float weight;
    float reward;

    OrderUrgency urgency;
    OrderStatus status;
};