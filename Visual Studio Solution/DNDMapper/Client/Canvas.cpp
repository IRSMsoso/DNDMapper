#include "Canvas.h"

Canvas::Canvas(){
	for (int y = 0; y < 20; y++) {
		std::vector<Tile> newRow;
		for (int  x= 0; x < 20; x++) {
			newRow.push_back(Tile(defaultColor));
		}
		tileGrid.push_back(newRow);
	}

	size = sf::Vector2i(20, 20);
}

Canvas::~Canvas(){

}

bool Canvas::paintTile(float worldx, float worldy, sf::Color newColor)
{
	unsigned int tileX = static_cast<int>(worldx / TILESIZE);
	unsigned int tileY = static_cast<int>(worldy / TILESIZE);

	if (tileY < tileGrid.size() && tileX < tileGrid.at(tileY).size()) {
		tileGrid.at(tileY).at(tileX).changeColor(newColor);
		return true;
	}
	return false;
}

bool Canvas::paintTile(sf::Vector2f worldxy, sf::Color newColor)
{
	unsigned int tileX = static_cast<int>(worldxy.x / TILESIZE);
	unsigned int tileY = static_cast<int>(worldxy.y / TILESIZE);

	if (tileY < tileGrid.size() && tileX < tileGrid.at(tileY).size()) {
		tileGrid.at(tileY).at(tileX).changeColor(newColor);
		return true;
	}
	return false;
}

bool Canvas::eraseTile(sf::Vector2f worldxy)
{
	unsigned int tileX = static_cast<int>(worldxy.x / TILESIZE);
	unsigned int tileY = static_cast<int>(worldxy.y / TILESIZE);

	if (tileY < tileGrid.size() && tileX < tileGrid.at(tileY).size()) {
		tileGrid.at(tileY).at(tileX).changeColor(defaultColor);
		return true;
	}

	return false;
}

std::vector<std::vector<Tile>>* Canvas::getTileGrid(){
	return &tileGrid;
}

//Helper Function. Expands the canvas to accommodate out of bounds clicks.
bool Canvas::expand()
{
	return false;
}

void Canvas::removeRow(unsigned int locY){
	tileGrid.erase(tileGrid.begin() + locY);
}

void Canvas::removeColumn(unsigned int locX){
	for (int y = 0; y < tileGrid.size(); y++) {
		tileGrid.at(y).erase(tileGrid.at(y).begin() + locX);
	}
}

void Canvas::addRowToBottom(){
	
}
