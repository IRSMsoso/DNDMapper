#include "Tile.h"


Tile::Tile(sf::Color newColor){
	changeColor(newColor);
	isFogged = false;

	needsUpdating = false;
}

Tile::~Tile(){

}

void Tile::changeColor(sf::Color newColor) {
	color = newColor;
	needsUpdating = true;
}

void Tile::setFog() {
	isFogged = true;
}

void Tile::removeFog() {
	isFogged = false;
}

bool Tile::checkFog() {
	if (isFogged) {
		return true;
	}
	return false;
}

sf::Color Tile::getColor()
{
	return color;
}
