#pragma once

#include <vector>
#include <cstddef>

#include "../Cell.h"

enum class PathDirection
{
    Forward,
    Backward
};

class Rover
{
public:
    Rover(
        int x,
        int y,
        int id,
        bool is_moving,
        float battery_level,
        float load
    );

    void setPosition(int new_x, int new_y);
    void setBatteryLevel(float level);
    void setLoad(float load);

    int getX() const;
    int getY() const;
    float getBatteryLevel() const;
    float getLoad() const;
    void setLoadMultiplier(float multiplier);
    int getId() const;

    void setPath(const std::vector<int>& new_path);

    void setPathDirection(PathDirection direction);

    void updateMovement(
        float delta_time,
        const std::vector<Cell>& map,
        int width
    );

    void setMoving(bool state);
    bool isMoving() const;

    const std::vector<int>& getPath() const;
    size_t getCurrentPathIndex() const;

    bool isDead() const;

private:
    int x;
    int y;
    int id;

    float battery_level;
    float load;
    float load_multiplier = 1.0f;

    bool moving = false;

    std::vector<int> path;

    size_t current_path_index = 0;

    float cell_timer = 0.0f;

    PathDirection path_direction =
        PathDirection::Forward;

    bool dead = false;
};