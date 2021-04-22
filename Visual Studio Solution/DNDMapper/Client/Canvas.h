#pragma once
#include <vector>
#include "Tile.h"
#include "Token.h"
#include "NetworkManager.h"
#include <iostream>
#include "DNDMapper.pb.h"
#include "ResourceManager.h"

const float TILESIZE = 25.f;
const int EXPANDDISTANCE = 8;
const sf::Vector2i MINSIZE = sf::Vector2i(85, 50);
const sf::Color defaultColor = sf::Color(55, 55, 55, 255);


class Canvas: public sf::Drawable {
public:
	Canvas(sf::View*, NetworkManager* newNetworkManager, ResourceManager* newResourceManager);
	~Canvas();

	bool paintTile(float x, float y, sf::Color, bool);
	bool paintTile(sf::Vector2f, sf::Color, bool);
	bool eraseTile(sf::Vector2f);

	bool fogTile(sf::Vector2f, bool);
	bool unfogTile(sf::Vector2f, bool);
	bool isFogged(sf::Vector2i);

	bool createToken(sf::Vector2f, sf::Color, bool);
	bool createToken(sf::Vector2f, sf::Color, sf::Uint16, bool);
	bool eraseToken(sf::Vector2f, bool);
	bool eraseToken(sf::Uint16);
	Token* getClickedToken(sf::Vector2f);
	Token* getTokenFromID(sf::Uint16);


	std::vector<std::vector<Tile>>* getTileGrid();

	void addRowToBottom(bool);
	void addColumnToRight(bool);
	void addRowToTop(bool);
	void addColumnToLeft(bool);

	void draw(sf::RenderTarget&, sf::RenderStates) const;

	void update();
	void reconstruct();


	bool expand();


	void saveMap(DNDProto::Map& map);
	bool loadMap(DNDProto::Map& map);

private:
	void removeRow(unsigned int);
	void removeColumn(unsigned int);

	void updateBead(int, int, int);

	std::vector<std::vector<Tile>> tileGrid; // Situated in a y, x format to simulate dealing with tiles from left to right, then top to bottom.

	sf::VertexArray tileVertexes;
	sf::VertexArray fogVertexes;
	sf::VertexArray beadVertexes;

	std::vector<sf::Vector2i> updateQueue;

	//Network Manager Pointer.
	NetworkManager* networkManager;
	//Resource Manager Pointer.
	ResourceManager* resourceManager;
	
	//List of tokens
	std::vector<Token> tokenList;

	//Pointer to Camera so that it can be moved upon expanding borders in negative coordinate directions
	sf::View* camera;
};