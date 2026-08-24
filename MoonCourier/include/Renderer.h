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
	
	void updateWindow(Simulation& simulation);
	
private:
	Font font;
	RenderWindow& window;
	int height;
	int width;
	float size_cell;

	void drawPathCells(
		const pathfinder::PathResult& path,
		const std::vector<Cell>& map,
		int widthGrid
	);
	void drawMap(const std::vector<Cell>& map);
	void drawBase(const Base& base);
	void drawOrders(const std::vector<Order>& orders);
	void drawRovers(const std::vector<Rover>& rovers);
	void drawMoney(float money);
	void drawBatteryLevel(const Rover& rover);
	void drawDeliveries(
		Simulation& simulation,
		int widthGrid
	);
	Color getCellColor(CellType type);	
	Color getPathColor(CellType type);


	sf::Color getBatteryColor(float battery) const;

	void drawBatteryBar(
		float battery,
		sf::Vector2f position,
		float width
	);

	void drawInfoPanel(
		Base& base,
		Simulation& simulation
	);
	void drawGameOver();
};
