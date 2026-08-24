#include <SFML/Graphics.hpp>

#include "include/Utils.h"
#include "include/Simulation.h"
#include "include/PathFinder.h"
#include "include/Renderer.h"

bool isClickedTarget(
    int pos_x,
    int pos_y,
    int target_x,
    int target_y
)
{
    return pos_x == target_x &&
        pos_y == target_y;
}

int main()
{
    float height =
        utils::writeData("height", 720.0f);

    float width =
        utils::writeData("width", 1280.0f);

    float size_cell =
        utils::writeData("size_cell", 20.0f);

    float speed_sim = utils::writeData("speed_sim", 1.0f);

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

    Clock clock;
    float accumulator = 0.0f;

    RenderWindow window(
        VideoMode({
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

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                continue;
            }

            const auto* key_event =
                event->getIf<Event::KeyPressed>();

            if (key_event)
            {
                Base& base =
                    simulation.getBase();

                switch (key_event->code)
                {
                case Keyboard::Key::Left:
                    base.selectPreviousRover();
                    break;

                case Keyboard::Key::Right:
                    base.selectNextRover();
                    break;

                case Keyboard::Key::C:
                {
                    Rover* rover =
                        base.getSelectedRover();

                    if (rover)
                    {
                        simulation.chargeRover(
                            rover->getId()
                        );
                    }

                    break;
                }

                case Keyboard::Key::B:
                    simulation.buyRover();
                    break;

                default:
                    break;
                }

                continue;
            }

            const auto* mouse_event =
                event->getIf<Event::MouseButtonPressed>();

            if (!mouse_event)
                continue;

            if (mouse_event->button !=
                Mouse::Button::Left)
            {
                continue;
            }

            const int mouse_x =
                static_cast<int>(
                    mouse_event->position.x / size_cell
                    );

            const int mouse_y =
                static_cast<int>(
                    mouse_event->position.y / size_cell
                    );

            Base& base =
                simulation.getBase();

            if (isClickedTarget(
                mouse_x,
                mouse_y,
                base.getX(),
                base.getY()))
            {
                continue;
            }

            for (Order& order :
                simulation.getOrders())
            {
                if (!isClickedTarget(
                    mouse_x,
                    mouse_y,
                    order.getX(),
                    order.getY()))
                {
                    continue;
                }

                Rover* rover =
                    base.getSelectedRover();

                if (!rover)
                    break;

                if (rover->isMoving())
                    break;

                auto path =
                    pathfinder::findPath(
                        simulation.getMap().getCells(),
                        width_grid,
                        height_grid,
                        rover->getX(),
                        rover->getY(),
                        order.getX(),
                        order.getY()
                    );

                if (!path.found)
                    break;

                simulation.createDelivery(
                    *rover,
                    order,
                    path
                );

                break;
            }
        }

        accumulator +=
            clock.restart().asSeconds();

        while (accumulator >= tick_rate)
        {
            accumulator -= tick_rate;

            simulation.update(
                tick_rate * speed_sim
            );
        }

        window.clear(
            Color::Black
        );

        renderer.updateWindow(
            simulation
        );

        window.display();
    }

    return 0;
}