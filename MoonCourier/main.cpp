#include <SFML/Graphics.hpp>

#include "include/Utils.h"
#include "include/Cell.h"
#include "include/Simulation.h"
#include "include/RandomGenerator.h"
#include "include/PathFinder.h"

#include <algorithm>
#include <random>
#include <vector>

void generateMap(std::vector<Cell>& map, int heightGrid, int widthGrid)
{
    std::mt19937 randomGenerator(std::random_device{}());

    std::discrete_distribution<int> terrainDistribution
    {
        50, // Regolith
        20, // LooseDust
        15, // CompactGround
        10, // RockyGround
        4,  // Crater
        1   // Impassable
    };

    map.clear();
    map.reserve(heightGrid * widthGrid);

    for (int y = 0; y < heightGrid; ++y)
    {
        for (int x = 0; x < widthGrid; ++x)
        {
            CellType type = static_cast<CellType>(
                terrainDistribution(randomGenerator)
                );

            map.emplace_back(x, y, type);
        }
    }
}

sf::Color getCellColor(CellType type)
{
    switch (type)
    {
    case CellType::Regolith:
        return sf::Color(105, 100, 92);

    case CellType::LooseDust:
        return sf::Color(135, 128, 116);

    case CellType::CompactGround:
        return sf::Color(78, 75, 70);

    case CellType::RockyGround:
        return sf::Color(58, 56, 53);

    case CellType::Crater:
        return sf::Color(38, 36, 35);

    case CellType::Impassable:
        return sf::Color(18, 17, 17);
    }

    return sf::Color(105, 100, 92);
}

void drawPathLines(
    sf::RenderWindow& window,
    const pathfinder::PathResult& path,
    int widthGrid,
    float sizeCell
)
{
    if (!path.found)
        return;

    constexpr float lineThickness = 3.0f;

    for (size_t i = 1; i < path.cells.size(); ++i)
    {
        auto [fromX, fromY] =
            utils::getCoordToIndex(path.cells[i - 1], widthGrid);

        auto [toX, toY] =
            utils::getCoordToIndex(path.cells[i], widthGrid);

        sf::RectangleShape line;
        line.setFillColor(sf::Color::White);

        if (fromX != toX)
        {
            int leftX = std::min(fromX, toX);

            line.setSize({ sizeCell, lineThickness });
            line.setOrigin({ 0.0f, lineThickness / 2.0f });

            line.setPosition(
                {
                    leftX * sizeCell + sizeCell / 2.0f,
                    fromY * sizeCell + sizeCell / 2.0f
                }
            );
        }
        else
        {
            int topY = std::min(fromY, toY);

            line.setSize({ lineThickness, sizeCell });
            line.setOrigin({ lineThickness / 2.0f, 0.0f });

            line.setPosition(
                {
                    fromX * sizeCell + sizeCell / 2.0f,
                    topY * sizeCell + sizeCell / 2.0f
                }
            );
        }

        window.draw(line);
    }
}

bool isOrderClicked(
    const Order& order,
    float mouseX,
    float mouseY,
    float sizeCell
)
{
    float orderX = order.getX() * sizeCell + sizeCell / 2.0f;
    float orderY = order.getY() * sizeCell + sizeCell / 2.0f;

    float deltaX = mouseX - orderX;
    float deltaY = mouseY - orderY;

    float radius = sizeCell / 3.0f;

    return deltaX * deltaX + deltaY * deltaY <= radius * radius;
}

int main()
{
    float height = utils::writeData("height", 720.0f);
    float width = utils::writeData("width", 1280.0f);
    float sizeCell = utils::writeData("size_cell", 20.0f);

    int heightGrid = static_cast<int>(height / sizeCell);
    int widthGrid = static_cast<int>(width / sizeCell);

    Simulation simulation(
        RandomGenerator::get(0, widthGrid - 1),
        RandomGenerator::get(0, heightGrid - 1),
        widthGrid,
        heightGrid,
        static_cast<int>(sizeCell),
        1000
    );

    std::vector<Cell> map;
    generateMap(map, heightGrid, widthGrid);

    // База всегда располагается на проходимой клетке.
    int baseIndex = utils::getIndexToCoord(
        simulation.getBaseX(),
        simulation.getBaseY(),
        widthGrid
    );

    map[baseIndex].setType(CellType::Regolith);

    // Заказы не должны появляться на непроходимой клетке.
    for (const Order& order : simulation.getOrders())
    {
        int orderIndex = utils::getIndexToCoord(
            order.getX(),
            order.getY(),
            widthGrid
        );

        map[orderIndex].setType(CellType::Regolith);
    }

    Rover& selectedRover = simulation.getRovers()[0];

    pathfinder::PathResult activePath;

    size_t roverPathStep = 0;
    bool roverMoving = false;

    float roverMoveAccumulator = 0.0f;
    constexpr float secondsPerCell = 0.15f;

    int tick = 60;
    float tickRate = 1.0f / tick;

    sf::Clock clock;
    float accumulator = 0.0f;

    sf::RenderWindow window(
        sf::VideoMode(
            {
                static_cast<unsigned int>(width),
                static_cast<unsigned int>(height)
            }
        ),
        "MoonCourier"
    );

    window.setVerticalSyncEnabled(true);

    sf::CircleShape baseShape(sizeCell / 2.0f);
    baseShape.setFillColor(sf::Color(70, 210, 100));
    baseShape.setOrigin({ sizeCell / 2.0f, sizeCell / 2.0f });
    baseShape.setPosition(
        {
            simulation.getBaseX() * sizeCell + sizeCell / 2.0f,
            simulation.getBaseY() * sizeCell + sizeCell / 2.0f
        }
    );

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mouseEvent =
                event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseEvent->button != sf::Mouse::Button::Left)
                    continue;

                for (const Order& order : simulation.getOrders())
                {
                    bool clicked = isOrderClicked(
                        order,
                        static_cast<float>(mouseEvent->position.x),
                        static_cast<float>(mouseEvent->position.y),
                        sizeCell
                    );

                    if (!clicked)
                        continue;

                    activePath = pathfinder::findPath(
                        map,
                        widthGrid,
                        heightGrid,
                        selectedRover.getX(),
                        selectedRover.getY(),
                        order.getX(),
                        order.getY()
                    );

                    roverPathStep = 1;
                    roverMoveAccumulator = 0.0f;

                    roverMoving =
                        activePath.found &&
                        activePath.cells.size() > 1;

                    break;
                }
            }
        }

        accumulator += clock.restart().asSeconds();

        while (accumulator >= tickRate)
        {
            accumulator -= tickRate;

            if (!roverMoving)
                continue;

            roverMoveAccumulator += tickRate;

            if (roverMoveAccumulator < secondsPerCell)
                continue;

            roverMoveAccumulator -= secondsPerCell;

            int cellIndex = activePath.cells[roverPathStep];

            auto [x, y] =
                utils::getCoordToIndex(cellIndex, widthGrid);

            selectedRover.setPosition(x, y);

            ++roverPathStep;

            if (roverPathStep >= activePath.cells.size())
                roverMoving = false;
        }

        window.clear(sf::Color(30, 30, 30));

        // Грунт.
        for (const Cell& cell : map)
        {
            sf::RectangleShape cellShape({ sizeCell, sizeCell });

            cellShape.setPosition(
                {
                    cell.getX() * sizeCell,
                    cell.getY() * sizeCell
                }
            );

            cellShape.setFillColor(getCellColor(cell.getType()));

            window.draw(cellShape);
        }

        // Белые линии текущего маршрута.
        drawPathLines(window, activePath, widthGrid, sizeCell);

        // База.
        window.draw(baseShape);

        // Метки заказов.
        for (const Order& order : simulation.getOrders())
        {
            sf::CircleShape orderShape(sizeCell / 4.0f);

            orderShape.setFillColor(sf::Color(225, 70, 65));
            orderShape.setOrigin({ sizeCell / 4.0f, sizeCell / 4.0f });

            orderShape.setPosition(
                {
                    order.getX() * sizeCell + sizeCell / 2.0f,
                    order.getY() * sizeCell + sizeCell / 2.0f
                }
            );

            window.draw(orderShape);
        }

        // Ровер.
        sf::CircleShape roverShape(sizeCell / 3.0f);

        roverShape.setFillColor(sf::Color(70, 160, 255));
        roverShape.setOrigin({ sizeCell / 3.0f, sizeCell / 3.0f });

        roverShape.setPosition(
            {
                selectedRover.getX() * sizeCell + sizeCell / 2.0f,
                selectedRover.getY() * sizeCell + sizeCell / 2.0f
            }
        );

        window.draw(roverShape);

        window.display();
    }

    return 0;
}