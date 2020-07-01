#pragma once
#include <vector>
#include "Tile.h"
#include "Token.h"
#include <iostream>

const float TILESIZE = 25.f;
const int EXPANDDISTANCE = 8;
const sf::Vector2i MINSIZE = sf::Vector2i(85, 50);
const sf::Color defaultColor = sf::Color(55, 55, 55, 255);


class Canvas: public sf::Drawable {
public:
	Canvas(sf::View*);
	~Canvas();

	bool paintTile(float x, float y, sf::Color);
	bool paintTile(sf::Vector2f, sf::Color);
	bool eraseTile(sf::Vector2f);

	bool fogTile(sf::Vector2f);
	bool unfogTile(sf::Vector2f);
	bool isFogged(sf::Vector2i);

	bool createToken(sf::Vector2f, sf::Color);
	bool eraseToken(sf::Vector2f);
	Token* getClickedToken(sf::Vector2f);


	std::vector<std::vector<Tile>>* getTileGrid();

	void addRowToBottom(bool);
	void addColumnToRight(bool);
	void addRowToTop(bool);
	void addColumnToLeft(bool);

	void draw(sf::RenderTarget&, sf::RenderStates) const;

	void update();
	void reconstruct();


	bool expand();

private:
	void removeRow(unsigned int);
	void removeColumn(unsigned int);

	void updateBead(int, int, int);

	std::vector<std::vector<Tile>> tileGrid; // Situated in a y, x format to simulate dealing with tiles from left to right, then top to bottom.

	sf::VertexArray tileVertexes;
	sf::VertexArray fogVertexes;
	sf::VertexArray beadVertexes;

	std::vector<sf::Vector2i> updateQueue;

	//Fog Cloud
	sf::Texture fogCloudTexture;

	
	//List of tokens
	std::vector<Token> tokenList;

	//Font for tokens
	sf::Font tokenFont;

	//Pointer to Camera so that it can be moved upon expanding borders in negative coordinate directions
	sf::View* camera;
};