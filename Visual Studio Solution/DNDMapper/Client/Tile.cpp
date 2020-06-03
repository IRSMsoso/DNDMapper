#include "Tile.h"

Tile::Tile(){
	sf::Color grey(55, 55, 55, 255);
	backgroundColor = grey;
}

Tile::Tile(sf::Color newColor){
	backgroundColor = newColor;
}

Tile::~Tile(){

}

void Tile::changeColor(sf::Color newColor){
	backgroundColor = newColor;
}

sf::Color Tile::getColor()
{
	return backgroundColor;
}
