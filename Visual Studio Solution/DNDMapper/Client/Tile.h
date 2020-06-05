#pragma once
#include "Token.h"
class Tile{
public:
	Tile(sf::Color);
	~Tile();

	void changeColor(sf::Color);
	void setFog();
	void removeFog();
	bool checkFog();
	sf::Color getColor();


private:
	sf::Color backgroundColor;
	std::vector<Token> tokenList;
	bool isFogged;
};

