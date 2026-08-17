#pragma once
#include <vector>
#include "Cell.h"
#include <random>

class Map {
public:

	Map(int width_grid, int height_grid);
	void generateRandomMap();
	int getHeightGrid() const;
	int getWidthGrid() const;
	void setCellType(int x, int y, CellType type);
	std::vector<Cell>& getCells();

private:
	std::vector<Cell> cells;
	int height_grid;
	int width_grid;
};