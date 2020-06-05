#include "Tile.h"


Tile::Tile(sf::Color newColor){
	backgroundColor = newColor;
	isFogged = false;
}

Tile::~Tile(){

}

void Tile::changeColor(sf::Color newColor) {
	backgroundColor = newColor;
}

void Tile::setFog() {
	isFogged = true;
}

bool Tile::checkFog() {
	if (isFogged) {
		return true;
	}
	return false;
}

sf::Color Tile::getColor()
{
	if (isFogged == true) {
		backgroundColor = sf::Color(255, 0, 0, 150);
	}
	return backgroundColor;
}
