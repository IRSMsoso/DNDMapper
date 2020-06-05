#pragma once
#include <vector>
#include "Tile.h"
#include "Token.h"

const float TILESIZE = 25.f;
const sf::Color defaultColor = sf::Color(55, 55, 55, 255);

class Canvas {
public:
	Canvas();
	~Canvas();

	bool paintTile(float x, float y, sf::Color);
	bool paintTile(sf::Vector2f, sf::Color);
	bool eraseTile(sf::Vector2f);

	std::vector<std::vector<Tile>>* getTileGrid();

private:
	bool expand();
	void removeRow(unsigned int);
	void removeColumn(unsigned int);
	void addRowToBottom();
	void addColumnToRight();

	std::vector<std::vector<Tile>> tileGrid; // Situated in a y, x format to simulate dealing with tiles from left to right, then top to bottom.
	sf::Vector2i size;
};