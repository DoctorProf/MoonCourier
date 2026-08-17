#pragma once
#include <utility>

class Rover
{
public:
	Rover(int x, int y, int id, bool isActive, float batteryLevel, float load);
	void setPosition(int newX, int newY);
	void setActive(bool active);
	void setBatteryLevel(float level);
	void setLoad(float load);
	int getX() const;
	int getY() const;
	bool getActive() const;
	float getBatteryLevel() const;
	float getLoad() const;
	int getId() const;

private:
	int x;
	int y;
	int id;
	bool isActive;
	float batteryLevel;
	float load;
	float speed;
};