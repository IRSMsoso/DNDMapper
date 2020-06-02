#include "Tile.h"

Tile::Tile(){
	backgroundColor = sf::Color::Black;
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
