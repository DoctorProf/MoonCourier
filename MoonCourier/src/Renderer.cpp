#include "../include/Renderer.h"

#include <algorithm>
#include <cstdint>
#include <iostream>

Renderer::Renderer(
    RenderWindow& window,
    int height,
    int width,
    float size_cell
)
    : window(window),
    height(height),
    width(width),
    size_cell(size_cell)
{
    if (!font.openFromFile(
        "assets/fonts/PixelOperator-Bold.ttf"))
    {
        std::cout << "Fonts not loaded!" << std::endl;
    }
}

Color Renderer::getCellColor(CellType type)
{
    switch (type)
    {
    case CellType::Regolith:
        return Color(105, 100, 92);

    case CellType::LooseDust:
        return Color(135, 128, 116);

    case CellType::CompactGround:
        return Color(78, 75, 70);

    case CellType::RockyGround:
        return Color(58, 56, 53);

    case CellType::Crater:
        return Color(38, 36, 35);

    case CellType::Impassable:
        return Color(18, 17, 17);
    }

    return Color(105, 100, 92);
}

Color Renderer::getPathColor(CellType type)
{
    switch (type)
    {
    case CellType::CompactGround:
        return Color(50, 220, 80);

    case CellType::Regolith:
        return Color(140, 220, 60);

    case CellType::LooseDust:
        return Color(230, 220, 50);

    case CellType::RockyGround:
        return Color(255, 140, 40);

    case CellType::Crater:
        return Color(230, 50, 50);

    default:
        return Color::White;
    }
}

Color Renderer::getBatteryColor(float battery) const
{
    battery = std::clamp(
        battery,
        0.0f,
        100.0f
    );

    float t = battery / 100.0f;

    std::uint8_t red =
        static_cast<std::uint8_t>(
            255.0f * (1.0f - t)
            );

    std::uint8_t green =
        static_cast<std::uint8_t>(
            255.0f * t
            );

    return Color(
        red,
        green,
        0
    );
}

void Renderer::drawMap(
    const std::vector<Cell>& map
)
{
    for (const Cell& cell : map)
    {
        RectangleShape cell_shape({
            size_cell,
            size_cell
            });

        cell_shape.setPosition({
            cell.getX() * size_cell,
            cell.getY() * size_cell
            });

        cell_shape.setFillColor(
            getCellColor(
                cell.getType()
            )
        );

        window.draw(cell_shape);
    }
}

void Renderer::drawBase(
    const Base& base
)
{
    RectangleShape shape;

    shape.setSize({
        size_cell,
        size_cell
        });

    shape.setFillColor(
        Color::Green
    );

    shape.setPosition({
        base.getX() * size_cell,
        base.getY() * size_cell
        });

    window.draw(shape);
}

void Renderer::drawOrders(
    const std::vector<Order>& orders
)
{
    for (const Order& order : orders)
    {
        CircleShape order_shape(
            size_cell / 4.0f
        );

        order_shape.setFillColor(
            Color(225, 70, 65)
        );

        order_shape.setOrigin({
            size_cell / 4.0f,
            size_cell / 4.0f
            });

        order_shape.setPosition({
            order.getX() * size_cell +
                size_cell / 2.0f,

            order.getY() * size_cell +
                size_cell / 2.0f
            });

        window.draw(order_shape);
    }
}

void Renderer::drawRovers(
    const std::vector<Rover>& rovers
)
{
    for (const Rover& rover : rovers)
    {
        CircleShape rover_shape(
            size_cell / 4.0f
        );

        rover_shape.setFillColor(
            Color(65, 70, 255)
        );

        rover_shape.setOrigin({
            size_cell / 4.0f,
            size_cell / 4.0f
            });

        rover_shape.setPosition({
            rover.getX() * size_cell +
                size_cell / 2.0f,

            rover.getY() * size_cell +
                size_cell / 2.0f
            });

        window.draw(rover_shape);
    }
}

void Renderer::drawPathCells(
    const pathfinder::PathResult& path,
    const std::vector<Cell>& map,
    int widthGrid
)
{
    if (!path.found)
        return;

    for (int cell_index : path.cells)
    {
        auto [x, y] =
            utils::getCoordToIndex(
                cell_index,
                widthGrid
            );

        RectangleShape cell;

        cell.setSize({
            size_cell - 2.0f,
            size_cell - 2.0f
            });

        cell.setPosition({
            x * size_cell + 1.0f,
            y * size_cell + 1.0f
            });

        cell.setFillColor(
            getPathColor(
                map[cell_index].getType()
            )
        );

        window.draw(cell);
    }
}

void Renderer::drawDeliveries(
    Simulation& simulation,
    int widthGrid
)
{
    const auto& deliveries =
        simulation.getDeliveries();

    const auto& map =
        simulation.getMap().getCells();

    for (const auto& delivery : deliveries)
    {
        drawPathCells(
            delivery.path,
            map,
            widthGrid
        );
    }
}

void Renderer::drawBatteryBar(
    float battery,
    Vector2f position,
    float width
)
{
    constexpr float height = 16.0f;

    battery = std::clamp(
        battery,
        0.0f,
        100.0f
    );

    RectangleShape background;

    background.setSize({
        width,
        height
        });

    background.setPosition(position);

    background.setFillColor(
        Color(45, 45, 45)
    );

    background.setOutlineColor(
        Color(120, 120, 120)
    );

    background.setOutlineThickness(1.0f);

    window.draw(background);

    RectangleShape fill;

    fill.setSize({
        width * battery / 100.0f,
        height
        });

    fill.setPosition(position);

    fill.setFillColor(
        getBatteryColor(battery)
    );

    window.draw(fill);
}

void Renderer::drawInfoPanel(
    Base& base,
    Simulation& simulation
)
{
    constexpr float panel_x = 10.0f;
    constexpr float panel_y = 10.0f;

    constexpr float panel_width = 320.0f;
    constexpr float panel_height = 225.0f;

    constexpr float padding = 15.0f;

    RectangleShape panel;

    panel.setSize({
        panel_width,
        panel_height
        });

    panel.setPosition({
        panel_x,
        panel_y
        });

    panel.setFillColor(
        Color(20, 20, 20, 122)
    );

    panel.setOutlineColor(
        Color(100, 100, 100)
    );

    panel.setOutlineThickness(2.0f);

    window.draw(panel);

    const Rover* rover =
        base.getSelectedRover();

    if (!rover)
    {
        Text money_text(font);

        money_text.setCharacterSize(20);
        money_text.setFillColor(
            Color::Green
        );

        money_text.setString(
            "MONEY: $" +
            std::to_string(
                static_cast<int>(
                    base.getMoney()
                    )
            )
        );

        money_text.setPosition({
            panel_x + padding,
            panel_y + padding
            });

        window.draw(money_text);

        Text no_rovers_text(font);

        no_rovers_text.setCharacterSize(20);
        no_rovers_text.setFillColor(
            Color::White
        );

        no_rovers_text.setString(
            "NO ROVERS"
        );

        no_rovers_text.setPosition({
            panel_x + padding,
            panel_y + 50.0f
            });

        window.draw(no_rovers_text);

        Text buy_text(font);

        buy_text.setCharacterSize(16);
        buy_text.setFillColor(
            Color::Yellow
        );

        buy_text.setString(
            "[B] BUY ROVER - $500"
        );

        buy_text.setPosition({
            panel_x + padding,
            panel_y + 90.0f
            });

        window.draw(buy_text);

        return;
    }

    Text money_text(font);

    money_text.setCharacterSize(20);
    money_text.setFillColor(
        Color::Green
    );

    money_text.setString(
        "MONEY: $" +
        std::to_string(
            static_cast<int>(
                base.getMoney()
                )
        )
    );

    money_text.setPosition({
        panel_x + padding,
        panel_y + padding
        });

    window.draw(money_text);

    Text rover_text(font);

    rover_text.setCharacterSize(20);
    rover_text.setFillColor(
        Color::White
    );

    rover_text.setString(
        "ROVER ID: " +
        std::to_string(
            rover->getId()
        )
    );

    rover_text.setPosition({
        panel_x + padding,
        panel_y + 45.0f
        });

    window.draw(rover_text);

    float battery =
        std::clamp(
            rover->getBatteryLevel(),
            0.0f,
            100.0f
        );

    Text battery_text(font);

    battery_text.setCharacterSize(18);
    battery_text.setFillColor(
        getBatteryColor(battery)
    );

    battery_text.setString(
        "BATTERY: " +
        std::to_string(
            static_cast<int>(battery)
        ) +
        "%"
    );

    battery_text.setPosition({
        panel_x + padding,
        panel_y + 72.0f
        });

    window.draw(battery_text);

    drawBatteryBar(
        battery,
        {
            panel_x + padding,
            panel_y + 98.0f
        },
        panel_width - padding * 2.0f
    );

    float reward = 0.0f;

    const auto& deliveries =
        simulation.getDeliveries();

    for (const auto& delivery : deliveries)
    {
        if (delivery.rover_id == rover->getId())
        {
            reward = delivery.reward;
            break;
        }
    }

    Text reward_text(font);

    reward_text.setCharacterSize(17);
    reward_text.setFillColor(
        reward > 0.0f
        ? Color::Yellow
        : Color(180, 180, 180)
    );

    reward_text.setString(
        "REWARD: $" +
        std::to_string(
            static_cast<int>(reward)
        )
    );

    reward_text.setPosition({
        panel_x + padding,
        panel_y + 119.0f
        });

    window.draw(reward_text);

    Text status_text(font);

    status_text.setCharacterSize(18);

    if (rover->isMoving())
    {
        status_text.setFillColor(
            Color::Red
        );

        status_text.setString(
            "STATUS: BUSY"
        );
    }
    else
    {
        status_text.setFillColor(
            Color::Green
        );

        status_text.setString(
            "STATUS: FREE"
        );
    }

    status_text.setPosition({
        panel_x + padding,
        panel_y + 143.0f
        });

    window.draw(status_text);

    Text controls_text(font);

    controls_text.setCharacterSize(14);
    controls_text.setFillColor(
        Color(200, 200, 200)
    );

    controls_text.setString(
        "< > SELECT ROVER"
    );

    controls_text.setPosition({
        panel_x + padding,
        panel_y + 170.0f
        });

    window.draw(controls_text);

    if (!rover->isMoving() &&
        rover->getX() == base.getX() &&
        rover->getY() == base.getY())
    {
        Text charge_text(font);

        charge_text.setCharacterSize(14);
        charge_text.setFillColor(
            Color::Yellow
        );

        charge_text.setString(
            "[C] CHARGE ROVER"
        );

        charge_text.setPosition({
            panel_x + 170.0f,
            panel_y + 170.0f
            });

        window.draw(charge_text);
    }
}

void Renderer::drawGameOver()
{
    RectangleShape overlay;

    overlay.setSize({
        static_cast<float>(width),
        static_cast<float>(height)
        });

    overlay.setPosition({
        0.0f,
        0.0f
        });

    overlay.setFillColor(
        Color(0, 0, 0, 190)
    );

    window.draw(overlay);

    Text game_over_text(font);

    game_over_text.setCharacterSize(48);
    game_over_text.setFillColor(
        Color::Red
    );

    game_over_text.setString(
        "GAME OVER"
    );

    FloatRect bounds =
        game_over_text.getLocalBounds();

    game_over_text.setOrigin({
        bounds.size.x / 2.0f,
        bounds.size.y / 2.0f
        });

    game_over_text.setPosition({
        width / 2.0f,
        height / 2.0f
        });

    window.draw(game_over_text);
}

void Renderer::updateWindow(
    Simulation& simulation
)
{
    drawMap(
        simulation.getMap().getCells()
    );

    drawDeliveries(
        simulation,
        simulation.getMap().getWidthGrid()
    );

    drawOrders(
        simulation.getOrders()
    );

    drawBase(
        simulation.getBase()
    );

    drawRovers(
        simulation.getBase().getRovers()
    );

    drawInfoPanel(
        simulation.getBase(),
        simulation
    );

    if (simulation.isGameOver())
    {
        drawGameOver();
    }
}