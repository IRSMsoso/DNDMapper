#pragma once
#include "Token.h"
class Tile {
public:
	Tile(sf::Color);
	~Tile();

	void changeColor(sf::Color);
	void setFog();
	void removeFog();
	bool checkFog();
	sf::Color getColor();


	bool needsUpdating;

private:
	std::vector<Token> tokenList;
	sf::Color color;
	bool isFogged;

	
};

