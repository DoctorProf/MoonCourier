#include "../../include/objects/Rover.h"

Rover::Rover(int x, int y, int id, bool isActive, float batteryLevel, float load_capacity)
	: x(x), y(y), id(id), isActive(isActive), batteryLevel(batteryLevel), load_capacity(load_capacity) 
{
}
void Rover::setPosition(int newX, int newY)
{
	x = newX;
	y = newY;
}
void Rover::setActive(bool active)
{
	isActive = active;
}
void Rover::setBatteryLevel(float level)
{
	batteryLevel = level;
}
void Rover::setLoadCapacity(float capacity)
{
	load_capacity = capacity;
}
int Rover::getId() const
{
	return id;
}
std::pair<int, int> Rover::getPosition() const
{
	return { x, y };
}
bool Rover::getActive() const
{
	return isActive;
}
float Rover::getBatteryLevel() const
{
	return batteryLevel;
}
float Rover::getLoadCapacity() const
{
	return load_capacity;
}
