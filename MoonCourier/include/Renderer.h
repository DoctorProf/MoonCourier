#pragma once
#include "PathFinder.h"
#include "Utils.h"
#include "Simulation.h"
#include <SFML/Graphics.hpp>
#include <memory>

using namespace sf;	

class Renderer
{
public:
	Renderer(RenderWindow& window,int height, int width, float size_cell);
	void drawPathLines(
		const pathfinder::PathResult& path,
		int widthGrid
	);
	void updateWindow(Simulation& simulation, const pathfinder::PathResult& activePath);
	
private:
	RenderWindow& window;
	int height;
	int width;
	float size_cell;
	void drawMap(const std::vector<Cell>& map);
	void drawBase(const Base& base);
	void drawOrders(const std::vector<Order>& orders);
	void drawRovers(const std::vector<Rover>& rovers);
	Color getCellColor(CellType type);	
};
