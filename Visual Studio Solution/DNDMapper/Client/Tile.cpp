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

bool Tile::shouldUpdate()
{
	return needsUpdating;
}

void Tile::setUpdate(bool newUpdate)
{
	needsUpdating = newUpdate;
}
