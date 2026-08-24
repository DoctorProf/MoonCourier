#include "../../include/models/Rover.h"

Rover::Rover(int x, int y, int id, bool isMoving, float batteryLevel, float load_capacity)
	: x(x), y(y), id(id), moving(isMoving), battery_level(batteryLevel), load(load_capacity) 
{
}
void Rover::setPosition(int newX, int newY)
{
	x = newX;
	y = newY;
}
void Rover::setBatteryLevel(float level)
{
	battery_level = level;
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
float Rover::getBatteryLevel() const
{
	return battery_level;
}
float Rover::getLoad() const
{
	return load;
}
void Rover::setLoadMultiplier(float multiplier)
{
    load_multiplier = multiplier;
}

void Rover::setPath(const std::vector<int>& new_path)
{
    path = new_path;

    current_path_index = 0;
    cell_timer = 0.0f;

    if (path.empty())
    {
        moving = false;
        return;
    }

    moving = true;
}

void Rover::updateMovement(
    float delta_time,
    const std::vector<Cell>& map,
    int width
)
{
    if (!moving || path.empty())
        return;

    if (path_direction == PathDirection::Forward)
    {
        if (current_path_index >= path.size() - 1)
        {
            moving = false;
            return;
        }
    }
    else
    {
        if (current_path_index == 0)
        {
            moving = false;
            return;
        }
    }

    size_t next_path_index;

    if (path_direction == PathDirection::Forward)
    {
        next_path_index = current_path_index + 1;
    }
    else
    {
        next_path_index = current_path_index - 1;
    }

    int next_cell_index =
        path[next_path_index];

    CellType next_type =
        map[next_cell_index].getType();

    float required_time =
        getCellTraversalTime(next_type)
        * load_multiplier;

    cell_timer += delta_time;

    if (cell_timer < required_time)
        return;

    cell_timer -= required_time;

    current_path_index = next_path_index;

    int new_x =
        next_cell_index % width;

    int new_y =
        next_cell_index / width;

    setPosition(new_x, new_y);

    battery_level -=
        getCellBatteryCost(next_type)
        * load_multiplier;

    if (battery_level <= 0.0f)
    {
        battery_level = 0.0f;
        dead = true;
        moving = false;
        return;
    }

    if (path_direction == PathDirection::Forward)
    {
        if (current_path_index >= path.size() - 1)
        {
            moving = false;
        }
    }
    else
    {
        if (current_path_index == 0)
        {
            moving = false;
        }
    }
}

void Rover::setMoving(bool state)
{
    moving = state;
}

bool Rover::isMoving() const
{
    return moving;
}

const std::vector<int>& Rover::getPath() const
{
    return path;
}

size_t Rover::getCurrentPathIndex() const
{
    return current_path_index;
}

void Rover::setPathDirection(PathDirection direction)
{
    path_direction = direction;
}

bool Rover::isDead() const
{
    return dead;
}