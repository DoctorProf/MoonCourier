#include "../../include/models/Base.h"

Base::Base(int x, int y, float money) : x(x), y(y), money(money) 
{
	Rover rover(x, y, RandomGenerator::get(0, 100), true, RandomGenerator::get(50.0f, 100.0f), 0.f);
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
Rover& Base::getRover(int rover_id) {
    auto it = std::find_if(
        rovers.begin(),
        rovers.end(),
        [rover_id](const Rover& rover)
        {
            return rover.getId() == rover_id;
        }
    );

    if (it == rovers.end())
        throw std::invalid_argument("Rover not found");

    return *it;
}
void Base::addRover(const Rover& rover)
{
	rovers.push_back(rover);
}
void Base::removeRover(int rover_id)
{
    auto it = std::find_if(
        rovers.begin(),
        rovers.end(),
        [rover_id](const Rover& rover)
        {
            return rover.getId() == rover_id;
        }
    );

    if (it == rovers.end())
        return;
    rovers.erase(it);
}
void Base::chargeRover(int rover_id)
{
    auto it = std::find_if(
        rovers.begin(),
        rovers.end(),
        [rover_id](const Rover& rover)
        {
            return rover.getId() == rover_id;
        }
    );

    if (it == rovers.end())
        return;

    (*it).setBatteryLevel(100.0f);
}

float Base::getMoney() const
{
	return money;
}
void Base::setMoney(float new_money)
{
	money = new_money;
}
