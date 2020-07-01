#pragma once
#include "Tile.h"
#include "Token.h"
#include <cereal/types/vector.hpp>


const float TILESIZE = 25.f;
const sf::Color defaultColor = sf::Color(55, 55, 55, 255);
const sf::Vector2i MINSIZE = sf::Vector2i(85, 50);
const int EXPANDDISTANCE = 8;


class Map{

public:
	Map();
	~Map();

	void setTile(sf::Vector2i, sf::Color);
	void setTile(sf::Vector2i, bool);

	void createToken(sf::Vector2f, sf::Color);
	bool eraseTile(sf::Uint16);

	Token* getToken(sf::Uint16);


	void addRowToBottom();
	void addColumnToRight();
	void addRowToTop();
	void addColumnToLeft();



private:

	std::vector<std::vector<Tile>> tileGrid; // Situated in a y, x format to simulate dealing with tiles from left to right, then top to bottom.

	std::vector<Token> tokenList;


	void removeRow(unsigned int);
	void removeColumn(unsigned int);


	//Serialization:
	friend class cereal::access;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(tileGrid, tokenList);
	}

};