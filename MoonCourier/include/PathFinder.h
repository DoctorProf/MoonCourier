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
        float totalCost = 0.0f;
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
        int startX,
        int startY,
        int targetX,
        int targetY
    )
    {
        PathResult result;

        if (width <= 0 || height <= 0)
            return result;

        if (map.size() != static_cast<size_t>(width * height))
            return result;

        auto isInBounds = [width, height](int x, int y)
            {
                return x >= 0 && x < width &&
                    y >= 0 && y < height;
            };

        if (!isInBounds(startX, startY) ||
            !isInBounds(targetX, targetY))
        {
            return result;
        }

        int startIndex = startY * width + startX;
        int targetIndex = targetY * width + targetX;

        if (!map[startIndex].isPassable() ||
            !map[targetIndex].isPassable())
        {
            return result;
        }

        const float infinity = std::numeric_limits<float>::infinity();

        std::vector<float> costs(map.size(), infinity);
        std::vector<int> previous(map.size(), -1);

        std::priority_queue<
            QueueNode,
            std::vector<QueueNode>,
            std::greater<QueueNode>
        > openCells;

        costs[startIndex] = 0.0f;
        openCells.push({ startIndex, 0.0f });

        const int directions[4][2] =
        {
            { 0, -1 },
            { 1, 0 },
            { 0, 1 },
            { -1, 0 }
        };

        while (!openCells.empty())
        {
            QueueNode current = openCells.top();
            openCells.pop();

            if (current.cost > costs[current.index])
                continue;

            if (current.index == targetIndex)
                break;

            int currentX = current.index % width;
            int currentY = current.index / width;

            for (const auto& direction : directions)
            {
                int neighborX = currentX + direction[0];
                int neighborY = currentY + direction[1];

                if (!isInBounds(neighborX, neighborY))
                    continue;

                int neighborIndex = neighborY * width + neighborX;
                const Cell& neighbor = map[neighborIndex];

                // Impassable сюда никогда не попадёт.
                if (!neighbor.isPassable())
                    continue;

                float newCost =
                    costs[current.index] +
                    getCellTraversalCost(neighbor.getType());

                if (newCost >= costs[neighborIndex])
                    continue;

                costs[neighborIndex] = newCost;
                previous[neighborIndex] = current.index;

                openCells.push({ neighborIndex, newCost });
            }
        }

        if (costs[targetIndex] == infinity)
            return result;

        for (int index = targetIndex; index != -1; index = previous[index])
        {
            result.cells.push_back(index);

            if (index == startIndex)
                break;
        }

        std::reverse(result.cells.begin(), result.cells.end());

        result.totalCost = costs[targetIndex];
        result.found = true;

        return result;
    }

    inline float getAverageDifficulty(const PathResult& path)
    {
        if (!path.found || path.cells.size() < 2)
            return 0.0f;

        return path.totalCost /
            static_cast<float>(path.cells.size() - 1);
    }
}