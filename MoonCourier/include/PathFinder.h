#pragma once
#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <vector>

#include "Cell.h"

namespace pathfinder
{
    struct PathResult
    {
        std::vector<int> cells;
        float total_cost = 0.0f;
        bool found = false;
    };

    struct QueueNode
    {
        int index;
        float cost;

        bool operator>(const QueueNode& other) const
        {
            return cost > other.cost;
        }
    };

    inline PathResult findPath(
        const std::vector<Cell>& map,
        int width,
        int height,
        int start_x,
        int start_y,
        int target_x,
        int target_y
    )
    {
        PathResult result;

        if (width <= 0 || height <= 0)
            return result;

        if (map.size() != static_cast<size_t>(width * height))
            return result;

        auto is_in_bounds = [width, height](int x, int y)
            {
                return x >= 0 && x < width &&
                    y >= 0 && y < height;
            };

        if (!is_in_bounds(start_x, start_y) ||
            !is_in_bounds(target_x, target_y))
        {
            return result;
        }

        int start_index = start_y * width + start_x;
        int target_index = target_y * width + target_x;

        if (!map[start_index].isPassable() ||
            !map[target_index].isPassable())
        {
            return result;
        }

        const float infinity =
            std::numeric_limits<float>::infinity();

        std::vector<float> costs(map.size(), infinity);
        std::vector<int> previous(map.size(), -1);

        std::priority_queue<
            QueueNode,
            std::vector<QueueNode>,
            std::greater<QueueNode>
        > open_cells;

        costs[start_index] = 0.0f;
        open_cells.push({ start_index, 0.0f });

        const int directions[4][2] =
        {
            { 0, -1 },
            { 1, 0 },
            { 0, 1 },
            { -1, 0 }
        };

        while (!open_cells.empty())
        {
            QueueNode current = open_cells.top();
            open_cells.pop();

            if (current.cost > costs[current.index])
                continue;

            if (current.index == target_index)
                break;

            int current_x = current.index % width;
            int current_y = current.index / width;

            for (const auto& direction : directions)
            {
                int neighbor_x = current_x + direction[0];
                int neighbor_y = current_y + direction[1];

                if (!is_in_bounds(neighbor_x, neighbor_y))
                    continue;

                int neighbor_index =
                    neighbor_y * width + neighbor_x;

                const Cell& neighbor =
                    map[neighbor_index];

                if (!neighbor.isPassable())
                    continue;

                float new_cost =
                    costs[current.index] +
                    getCellTraversalCost(
                        neighbor.getType()
                    );

                if (new_cost >= costs[neighbor_index])
                    continue;

                costs[neighbor_index] = new_cost;
                previous[neighbor_index] = current.index;

                open_cells.push(
                    { neighbor_index, new_cost }
                );
            }
        }

        if (costs[target_index] == infinity)
            return result;

        for (int index = target_index;
            index != -1;
            index = previous[index])
        {
            result.cells.push_back(index);

            if (index == start_index)
                break;
        }

        std::reverse(
            result.cells.begin(),
            result.cells.end()
        );

        result.total_cost = costs[target_index];
        result.found = true;

        return result;
    }

    inline float getAverageDifficulty(
        const PathResult& path
    )
    {
        if (!path.found || path.cells.size() < 2)
            return 0.0f;

        return path.total_cost /
            static_cast<float>(path.cells.size() - 1);
    }
}