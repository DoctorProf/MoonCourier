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

void Renderer::drawPathLines(
	const pathfinder::PathResult& path,
	int widthGrid
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

            line.setSize({ size_cell, lineThickness });
            line.setOrigin({ 0.0f, lineThickness / 2.0f });

            line.setPosition(
                {
                    leftX * size_cell + size_cell / 2.0f,
                    fromY * size_cell + size_cell / 2.0f
                }
            );
        }
        else
        {
            int topY = std::min(fromY, toY);

            line.setSize({ lineThickness, size_cell });
            line.setOrigin({ lineThickness / 2.0f, 0.0f });

            line.setPosition(
                {
                    fromX * size_cell + size_cell / 2.0f,
                    topY * size_cell + size_cell  / 2.0f
                }
            );
        }

        window.draw(line);
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


void Renderer::updateWindow(Simulation& simulation, const pathfinder::PathResult& activePath)
{
    drawMap(simulation.getMap().getCells());
	drawPathLines(activePath, simulation.getMap().getWidthGrid());
	drawBase(simulation.getBase());
	drawOrders(simulation.getOrders());
    drawRovers(simulation.getBase().getRovers());

}