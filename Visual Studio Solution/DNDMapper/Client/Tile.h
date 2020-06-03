#pragma once
#include "Token.h"
class Tile{
public:
	Tile();
	Tile(sf::Color);
	~Tile();

	void changeColor(sf::Color);
	sf::Color getColor();

private:
	sf::Color backgroundColor;
	std::vector<Token> tokenList;
};

