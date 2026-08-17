#include "../../include/models/Rover.h"

Rover::Rover(int x, int y, int id, bool isActive, float batteryLevel, float load_capacity)
	: x(x), y(y), id(id), isActive(isActive), batteryLevel(batteryLevel), load(load_capacity) 
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
void Rover::setLoad(float capacity)
{
	load = capacity;
}
int Rover::getId() const
{
	return id;
}
int Rover::getX() const
{
	return x;
}
int Rover::getY() const
{
	return y;
}
bool Rover::getActive() const
{
	return isActive;
}
float Rover::getBatteryLevel() const
{
	return batteryLevel;
}
float Rover::getLoad() const
{
	return load;
}
