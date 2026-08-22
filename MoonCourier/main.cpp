#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

#include "include/Utils.h"
#include "include/Simulation.h"
#include "include/PathFinder.h"
#include "include/Renderer.h"

#include <cstddef>

bool isClickedTarget(
    int pos_x,
    int pos_y,
    int target_x,
    int target_y
)
{
    return pos_x == target_x && pos_y == target_y;
}

int main()
{
    float height = utils::writeData("height", 720.0f);
    float width = utils::writeData("width", 1280.0f);
    float size_cell = utils::writeData("size_cell", 20.0f);

    int height_grid =
        static_cast<int>(height / size_cell);

    int width_grid =
        static_cast<int>(width / size_cell);

    Simulation simulation(
        width_grid,
        height_grid
    );

    constexpr int tick = 60;
    constexpr float tick_rate = 1.0f / tick;

    sf::Clock clock;
    float accumulator = 0.0f;

    sf::RenderWindow window(
        sf::VideoMode({
            static_cast<unsigned int>(width),
            static_cast<unsigned int>(height)
            }),
        "MoonCourier"
    );

    window.setVerticalSyncEnabled(true);

    Renderer renderer(
        window,
        height,
        width,
        size_cell
    );

    tgui::Gui gui(window);

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            bool gui_consumed = gui.handleEvent(*event);

            if (event->is<sf::Event::Closed>())
            {
                window.close();
                continue;
            }

            if (gui_consumed)
                continue;

            const auto* mouse_event =
                event->getIf<sf::Event::MouseButtonPressed>();

            if (!mouse_event)
                continue;

            if (mouse_event->button != sf::Mouse::Button::Left)
                continue;

            const int mouse_x =
                static_cast<int>(
                    mouse_event->position.x / size_cell
                    );

            const int mouse_y =
                static_cast<int>(
                    mouse_event->position.y / size_cell
                    );

            Base& base = simulation.getBase();

            // Не позволяем нажимать на базу
            if (isClickedTarget(
                mouse_x,
                mouse_y,
                base.getX(),
                base.getY()))
            {
                continue;
            }

            for (Order& order : simulation.getOrders())
            {
                if (!isClickedTarget(
                    mouse_x,
                    mouse_y,
                    order.getX(),
                    order.getY()))
                {
                    continue;
                }

                // Пока у нас один тестовый ровер
                Rover& rover = base.getRovers()[0];

                if (!rover.getActive())
                    break;

                if (rover.isMoving())
                    break;

                auto path = pathfinder::findPath(
                    simulation.getMap().getCells(),
                    width_grid,
                    height_grid,

                    rover.getX(),
                    rover.getY(),

                    order.getX(),
                    order.getY()
                );

                if (!path.found)
                    break;

                simulation.createDelivery(
                    rover,
                    order,
                    path
                );

                break;
            }
        }

        accumulator += clock.restart().asSeconds();

        while (accumulator >= tick_rate)
        {
            accumulator -= tick_rate;

            simulation.update(tick_rate);
        }

        window.clear(sf::Color(0, 0, 0));

        renderer.updateWindow(simulation );

        gui.draw();

        window.display();
    }

    return 0;
}