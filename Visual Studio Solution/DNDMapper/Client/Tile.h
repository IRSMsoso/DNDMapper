#pragma once
#include "Token.h"
class Tile {
public:
	Tile(sf::Color);
	~Tile();

	void changeColor(sf::Color);
	void setFog(bool);
	bool getFog();
	sf::Color getColor();



private:
	std::vector<Token> tokenList;
	sf::Color color;
	bool isFogged;

};

