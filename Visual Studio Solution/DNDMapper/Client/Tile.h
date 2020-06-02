#pragma once
#include <SFML/Graphics.hpp>
class Tile{
public:
	Tile();
	~Tile();

	void changeColor(sf::Color);
	sf::Color getColor();

private:
	sf::Color backgroundColor;
	//Occupying Token
};

