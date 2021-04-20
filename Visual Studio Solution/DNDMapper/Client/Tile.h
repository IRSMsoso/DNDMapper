#pragma once
#include <SFML/Graphics.hpp>

class Tile {
public:
	Tile(sf::Color);
	~Tile();

	void changeColor(sf::Color);
	void setFog(bool);
	bool getFog();
	sf::Color getColor();



private:
	sf::Color color;
	bool isFogged;

};

