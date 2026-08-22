#include "../include/Renderer.h"

Renderer::Renderer(RenderWindow& window, int height, int width, float size_cell) : window(window), height(height), width(width), size_cell(size_cell)
{

}
Color Renderer::getCellColor(CellType type)
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
            utils::getCoordToIndex(cell_index, widthGrid);

        sf::RectangleShape cell;
        cell.setSize({
            size_cell - 2.0f,
            size_cell - 2.0f
            });

        cell.setPosition({
            x * size_cell + 1.0f,
            y * size_cell + 1.0f
            });

        cell.setFillColor(
            getPathColor(map[cell_index].getType())
        );

        window.draw(cell);
    }
}
void Renderer::drawMap(const std::vector<Cell>& map)
{
    for (const Cell& cell : map)
    {
        sf::RectangleShape cellShape({  size_cell, size_cell });

        cellShape.setPosition(
            {
                cell.getX() * size_cell,
                cell.getY() * size_cell
            }
        );

        cellShape.setFillColor(getCellColor(cell.getType()));

        window.draw(cellShape);
    }
}
void Renderer::drawBase(const Base& base)
{
    sf::RectangleShape shape;
    shape.setSize({ size_cell, size_cell });
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin({ 0.0f, 0.0f });
    shape.setPosition(
        {
            base.getX() * size_cell,
            base.getY() * size_cell
        }
    );
    window.draw(shape);
}
void Renderer::drawOrders(const std::vector<Order>& orders)
{
    for (const Order& order : orders)
    {
        sf::CircleShape orderShape(size_cell / 4.0f);

        orderShape.setFillColor(sf::Color(225, 70, 65));
        orderShape.setOrigin({ size_cell / 4.0f, size_cell / 4.0f });

        orderShape.setPosition(
            {
                order.getX() * size_cell + size_cell / 2.0f,
                order.getY() * size_cell + size_cell / 2.0f
            }
        );

        window.draw(orderShape);
    }
}
void Renderer::drawRovers(const std::vector<Rover>& rovers) 
{
    for (const Rover& rover : rovers)
    {
        sf::CircleShape rover_shape(size_cell / 4.0f);

        rover_shape.setFillColor(sf::Color(65, 70, 255));
        rover_shape.setOrigin({ size_cell / 4.0f, size_cell / 4.0f });

        rover_shape.setPosition(
            {
                rover.getX() * size_cell + size_cell / 2.0f,
                rover.getY() * size_cell + size_cell / 2.0f
            }
        );

        window.draw(rover_shape);
    }
}
void Renderer::drawMoney(float money) {

    sf::Font font;
    if (!font.openFromFile("assets/fonts/PixelOperator-Bold.ttf"))
    {
		std::cout << "Fonts not loaded!" << std::endl;
        return;
    }

    sf::Text money_text(font);
    money_text.setString("MONEY: $" + std::to_string(static_cast<int>(money)));
    money_text.setCharacterSize(24);
    money_text.setFillColor(sf::Color::Green);
    money_text.setPosition({ 10.0f, 10.0f });

    window.draw(money_text);
}


void Renderer::updateWindow(Simulation& simulation)
{
    drawMap(simulation.getMap().getCells());
	drawDeliveries(simulation, simulation.getMap().getWidthGrid());
    drawRovers(simulation.getBase().getRovers());
	drawBase(simulation.getBase());
	drawOrders(simulation.getOrders());
	drawMoney(simulation.getBase().getMoney());
}
void Renderer::drawDeliveries(
    Simulation& simulation,
    int widthGrid
)
{
    const auto& deliveries = simulation.getDeliveries();
    const auto& map = simulation.getMap().getCells();

    for (const auto& delivery : deliveries)
    {
        drawPathCells(
            delivery.path,
            map,
            widthGrid
        );
    }
}

Color Renderer::getPathColor(CellType type)
{
    switch (type)
    {
    case CellType::CompactGround:
        return sf::Color(50, 220, 80);

    case CellType::Regolith:
        return sf::Color(140, 220, 60);

    case CellType::LooseDust:
        return sf::Color(230, 220, 50);

    case CellType::RockyGround:
        return sf::Color(255, 140, 40);

    case CellType::Crater:
        return sf::Color(230, 50, 50);

    default:
        return sf::Color::White;
    }
}