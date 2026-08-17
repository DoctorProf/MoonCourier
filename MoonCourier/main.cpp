#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

#include "include/Utils.h"
#include "include/Simulation.h"
#include "include/PathFinder.h"
#include "include/Renderer.h"
#include "include/BasePanelGui.h"

#include <cstddef>

bool isClickedTarget(int pos_x, int pos_y, int target_x, int target_y)
{
    return pos_x == target_x && pos_y == target_y;
}
int main()
{
    float height = utils::writeData("height", 720.0f);
    float width = utils::writeData("width", 1280.0f);
    float size_cell = utils::writeData("size_cell", 20.0f);

    int height_grid = static_cast<int>(height / size_cell);
    int width_grid = static_cast<int>(width / size_cell);

    Simulation simulation(width_grid, height_grid);

    pathfinder::PathResult activePath;

    std::size_t roverPathStep = 0;
    bool roverMoving = false;

    float roverMoveAccumulator = 0.0f;
    constexpr float secondsPerCell = 0.15f;

    constexpr int tick = 60;
    constexpr float tickRate = 1.0f / tick;

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

    Renderer renderer(window, height, width, size_cell);

    tgui::Gui gui(window);

    BasePanelGui basePanel(
        gui,
        simulation.getBase(),
        width,
        height
    );

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            gui.handleEvent(*event);

            if (event->is<sf::Event::Closed>())
            {
                window.close();
                continue;
            }

            const auto* mouse_event =
                event->getIf<sf::Event::MouseButtonPressed>();

            if (!mouse_event ||
                mouse_event->button != sf::Mouse::Button::Left)
            {
                continue;
            }

            float mouse_x = static_cast<float>(mouse_event->position.x);
            float mouse_y = static_cast<float>(mouse_event->position.y);

            Base& base = simulation.getBase();

            if (isClickedTarget((int)mouse_x / size_cell, (int)mouse_y / size_cell, base.getX(), base.getY()))
            {
                basePanel.toggle();
                continue;
            }

            Rover* selectedRover = base.getSelectedRover();

            if (!selectedRover)
                continue;

            for (const Order& order : simulation.getOrders())
            {
                if (!isClickedTarget((int)mouse_x / size_cell, (int)mouse_y / size_cell, order.getX(), order.getY()))
                    continue;

                activePath = pathfinder::findPath(
                    simulation.getMap().getCells(),
                    width_grid,
                    height_grid,
                    selectedRover->getX(),
                    selectedRover->getY(),
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

        accumulator += clock.restart().asSeconds();

        while (accumulator >= tickRate)
        {
            accumulator -= tickRate;

            if (!roverMoving)
                continue;

            Rover* selectedRover =
                simulation.getBase().getSelectedRover();

            if (!selectedRover)
            {
                roverMoving = false;
                continue;
            }

            roverMoveAccumulator += tickRate;

            if (roverMoveAccumulator < secondsPerCell)
                continue;

            roverMoveAccumulator -= secondsPerCell;

            int cellIndex = activePath.cells[roverPathStep];

            auto [x, y] = utils::getCoordToIndex(
                cellIndex,
                width_grid
            );

            selectedRover->setPosition(x, y);

            ++roverPathStep;

            if (roverPathStep >= activePath.cells.size())
                roverMoving = false;
        }

        window.clear(sf::Color(30, 30, 30));

        renderer.updateWindow(simulation, activePath);

        gui.draw();

        window.display();
    }

    return 0;
}