#include "Map.h"

Map::Map()
{
}

Map::~Map()
{
}

void Map::setTile(sf::Vector2i xy, sf::Color newColor){
	tileGrid.at(xy.y).at(xy.x).changeColor(newColor);
}

void Map::setTile(sf::Vector2i xy, bool fog){
	tileGrid.at(xy.y).at(xy.x).setFog(fog);
}

void Map::createToken(sf::Vector2f worldxy, sf::Color newColor, sf::Uint16 id){
	tokenList.push_back(Token(newColor, worldxy, id));
}

bool Map::eraseTile(sf::Uint16 eraseID)
{

	for (int i = 0; i < tokenList.size(); i++) {
		if (tokenList.at(i).getID() == eraseID) {
			tokenList.erase(tokenList.begin() + i);
			return true;
		}
	}

	return false;
}

Token * Map::getToken(sf::Uint16 getID)
{

	for (int i = 0; i < tokenList.size(); i++) {
		if (tokenList.at(i).getID() == getID) {
			return &tokenList.at(i);
		}
	}

	return nullptr;
}

void Map::removeRow(unsigned int locY) {
	tileGrid.erase(tileGrid.begin() + locY);
}

void Map::removeColumn(unsigned int locX) {
	for (int y = 0; y < tileGrid.size(); y++) {
		tileGrid.at(y).erase(tileGrid.at(y).begin() + locX);
	}
}

void Map::addRowToBottom() {
	std::vector<Tile> newRow;
	for (int i = 0; i < tileGrid.at(0).size(); i++) {
		newRow.push_back(Tile(defaultColor));
		//std::cout << "Pushed new row\n";
	}
	tileGrid.push_back(newRow);
}

void Map::addColumnToRight() {
	for (int i = 0; i < tileGrid.size(); i++) {
		tileGrid.at(i).push_back(Tile(defaultColor));
		//std::cout << "Pushed new column\n";
	}
}

void Map::addRowToTop() {
	std::vector<Tile> newRow;
	for (int i = 0; i < tileGrid.at(0).size(); i++) {
		newRow.push_back(Tile(defaultColor));
	}

	tileGrid.insert(tileGrid.begin(), newRow);

	for (int i = 0; i < tokenList.size(); i++) {
		tokenList.at(i).setPosition(tokenList.at(i).getPosition() + sf::Vector2f(0.f, 25.f));
	}
}

void Map::addColumnToLeft() {
	for (int i = 0; i < tileGrid.size(); i++) {
		tileGrid.at(i).insert(tileGrid.at(i).begin(), Tile(defaultColor));
	}

	for (int i = 0; i < tokenList.size(); i++) {
		tokenList.at(i).setPosition(tokenList.at(i).getPosition() + sf::Vector2f(25.f, 0.f));
	}
}