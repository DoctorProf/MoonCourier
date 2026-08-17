#include "../include/Map.h"

Map::Map(int width, int height) : width_grid(width), height_grid(height) 
{
	generateRandomMap();
}

void Map::generateRandomMap() 
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

    cells.clear();
    cells.reserve(height_grid * width_grid);

    for (int y = 0; y < height_grid; ++y)
    {
        for (int x = 0; x < width_grid; ++x)
        {
            CellType type = static_cast<CellType>(
                terrainDistribution(randomGenerator)
                );

            cells.emplace_back(x, y, type);
        }
    }
}

int Map::getHeightGrid() const {
	return height_grid;
}

int Map::getWidthGrid() const {
	return width_grid;
}
void Map::setCellType(int x, int y, CellType type)
{
    if (x >= 0 && x < width_grid && y >= 0 && y < height_grid)
    {
        cells[y * width_grid + x].setType(type);
    }
}
std::vector<Cell>& Map::getCells() {
        return cells;
}