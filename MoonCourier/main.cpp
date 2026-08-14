#include <iostream>
#include <SFML/Graphics.hpp>
#include "include/Utils.h"
#include "include/Cell.h"
#include <vector>
#include <random>

void generateMap(std::vector<Cell>& map, int height_grid, int width_grid)
{
    std::mt19937 randomGenerator(std::random_device{}());

    std::discrete_distribution<int> terrain_distribution
    {
        50, // Regolith
        20, // LooseDust
        15, // CompactGround
        10, // RockyGround
        4,  // Crater
        1   // Impassable
    };

    map.clear();
    map.reserve(height_grid * width_grid);

    for (int y = 0; y < height_grid; ++y)
    {
        for (int x = 0; x < width_grid; ++x)
        {
            CellType type =
                static_cast<CellType>(terrain_distribution(randomGenerator));

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

int main()
{

    float height = utils::writeData("Height", 720);
    float width = utils::writeData("Width", 1280);
	float size_cell = utils::writeData("SizeCell", 20);
    

    int tick = 60;

	int height_grid = static_cast<int>(height / size_cell);
	int width_grid = static_cast<int>(width / size_cell);

    float tick_rate = 1.f / tick;

    sf::Clock clock;
    float accumulator = 0.f;

    sf::RenderWindow window(
        sf::VideoMode({ static_cast<unsigned int>(width), static_cast<unsigned int>(height) }),
        "MoonCourier"
    );

    window.setVerticalSyncEnabled(true);

	std::vector<Cell> map;
	generateMap(map, height_grid, width_grid);

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        accumulator += clock.restart().asSeconds();

        while (accumulator >= tick_rate)
        {

            accumulator -= tick_rate;
        }

        window.clear(sf::Color(30, 30, 30));

		for (auto cell : map)
		{
			sf::RectangleShape cellShape({ size_cell, size_cell });
			cellShape.setPosition({ cell.getX() * size_cell, cell.getY() * size_cell});
			cellShape.setFillColor(getCellColor(cell.getType()));
			window.draw(cellShape);
		}
        window.display();
    }

    return 0;
}