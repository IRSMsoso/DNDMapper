#include "Tile.h"


Tile::Tile(sf::Color newColor) {
	changeColor(newColor);

	isFogged = false;
}

Tile::~Tile() {

}

void Tile::changeColor(sf::Color newColor) {
	color = newColor;
}

void Tile::setFog(bool newFog) {
	isFogged = newFog;
}


bool Tile::getFog() {
	if (isFogged) {
		return true;
	}
	return false;
}

sf::Color Tile::getColor()
{
	return color;
}
