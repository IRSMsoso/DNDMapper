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
	return backgroundColor;
}
