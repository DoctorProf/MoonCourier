#include "../../include/models/Base.h"

Base::Base(int x, int y, float money) : x(x), y(y), money(money) 
{
	Rover rover(x, y, 0, true, RandomGenerator::get(50.0f, 100.0f), RandomGenerator::get(10.0f, 50.0f));
	rovers.push_back(rover);
}

int Base::getX() const
{
	return x;
}
int Base::getY() const
{
	return y;
}
std::vector<Rover>& Base::getRovers()
{
	return rovers;
}

const std::vector<Rover>& Base::getRovers() const
{
	return rovers;
}
void Base::addRover(const Rover& rover)
{
	rovers.push_back(rover);
}
void Base::removeRover(int rover_id)
{
    if (rover_id == selected_rover_id)
    {
        selected_rover_id = -1;
    }
    auto it = std::find_if(
        rovers.begin(),
        rovers.end(),
        [this](const Rover& rover)
        {
            return rover.getId() == selected_rover_id;
        }
    );

    if (it == rovers.end())
        return;
    rovers.erase(it);
}
float Base::getMoney() const
{
	return money;
}
void Base::setMoney(float newMoney)
{
	money = newMoney;
}

#include <algorithm>

bool Base::selectRover(int roverId)
{
    auto it = std::find_if(
        rovers.begin(),
        rovers.end(),
        [roverId](const Rover& rover)
        {
            return rover.getId() == roverId;
        }
    );

    if (it == rovers.end())
        return false;

    selected_rover_id = roverId;
    return true;
}

Rover* Base::getSelectedRover()
{
    auto it = std::find_if(
        rovers.begin(),
        rovers.end(),
        [this](const Rover& rover)
        {
            return rover.getId() == selected_rover_id;
        }
    );

    if (it == rovers.end())
        return nullptr;

    return &(*it);
}

const Rover* Base::getSelectedRover() const
{
    auto it = std::find_if(
        rovers.begin(),
        rovers.end(),
        [this](const Rover& rover)
        {
            return rover.getId() == selected_rover_id;
        }
    );

    if (it == rovers.end())
        return nullptr;

    return &(*it);
}

bool Base::chargeSelectedRover(float price)
{
    Rover* rover = getSelectedRover();

    if (!rover)
        return false;

    if (price <= 0.0f)
        return false;

    if (money < price)
        return false;

    if (rover->getBatteryLevel() >= 100.0f)
        return false;

    money -= price;
    rover->setBatteryLevel(100.0f);

    return true;
}