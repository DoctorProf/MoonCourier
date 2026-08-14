#pragma once
#include <utility>

class Rover
{
public:
	Rover(int x, int y, int id, bool isActive, float batteryLevel, float load_capacity);
	void setPosition(int newX, int newY);
	void setActive(bool active);
	void setBatteryLevel(float level);
	void setLoadCapacity(float capacity);
	std::pair<int, int> getPosition() const;
	bool getActive() const;
	float getBatteryLevel() const;
	float getLoadCapacity() const;
	int getId() const;

private:
	int x;
	int y;
	int id;
	bool isActive;
	float batteryLevel;
	float load_capacity;
};