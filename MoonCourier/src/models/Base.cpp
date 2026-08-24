#include "../../include/models/Base.h"

Base::Base(int x, int y, float money) : x(x), y(y), money(money) 
{
    for (int i = 0; i < 3; ++i)
    {
        Rover rover(
            x,
            y,
            RandomGenerator::get(0, 10000),
            false,
            RandomGenerator::get(50.0f, 100.0f),
            0.0f
        );

        rovers.push_back(rover);
    }
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

    if (rovers.size() == 1)
        selected_rover_index = 0;
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

    int removed_index =
        static_cast<int>(
            std::distance(rovers.begin(), it)
            );

    rovers.erase(it);

    if (rovers.empty())
    {
        selected_rover_index = 0;
        return;
    }

    if (removed_index < selected_rover_index)
    {
        --selected_rover_index;
    }

    if (selected_rover_index >=
        static_cast<int>(rovers.size()))
    {
        selected_rover_index =
            static_cast<int>(rovers.size()) - 1;
    }
}
bool Base::chargeRover(int rover_id)
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
        return false;

    float current_battery = it->getBatteryLevel();
    float charge_cost = 100.0f - current_battery;

    if (charge_cost <= 0.0f)
        return true;

    if (money < charge_cost)
        return false;

    money -= charge_cost;

    it->setBatteryLevel(100.0f);
    return true;
}
void Base::addMoney(float amount) {
    money += amount;
}
float Base::getMoney() const
{
	return money;
}
void Base::setMoney(float new_money)
{
	money = new_money;
}

Rover* Base::getSelectedRover()
{
    if (rovers.empty())
        return nullptr;

    if (selected_rover_index >= static_cast<int>(rovers.size()))
        selected_rover_index = 0;

    return &rovers[selected_rover_index];
}

void Base::selectNextRover()
{
    if (rovers.empty())
        return;

    selected_rover_index++;

    if (selected_rover_index >= static_cast<int>(rovers.size()))
        selected_rover_index = 0;
}

void Base::selectPreviousRover()
{
    if (rovers.empty())
        return;

    selected_rover_index--;

    if (selected_rover_index < 0)
        selected_rover_index =
        static_cast<int>(rovers.size()) - 1;
}