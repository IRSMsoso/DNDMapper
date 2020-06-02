#pragma once
#include <vector>
#include "Tile.h"


class Canvas {
public:
	Canvas();
	~Canvas();

	bool paintTile(float x, float y, sf::Color);

	std::vector<std::vector<Tile>>* getTileGrid();

private:
	std::vector<std::vector<Tile>> tileGrid; // Situated in a y, x format to simulate dealing with tiles from left to right, then top to bottom.
};