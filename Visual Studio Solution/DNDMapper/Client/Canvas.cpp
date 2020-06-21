#include "Canvas.h"

Canvas::Canvas(){
	sf::Clock setupClock;


	std::vector<Tile> firstRow;
	firstRow.push_back(Tile(defaultColor));
	tileGrid.push_back(firstRow);

	for (int y = 0; y < 2; y++) {
		addRowToBottom(false);
		addColumnToRight(false);
	}
	reconstruct();

	size = sf::Vector2i(20, 20);


	fogCloudTexture.loadFromFile("FogCloud.png");

	std::cout << "Canvas setup took " << setupClock.getElapsedTime().asSeconds() << " seconds.\n";
}

Canvas::~Canvas(){

}

//Same call as other paintTile function
bool Canvas::paintTile(float worldx, float worldy, sf::Color newColor)
{
	return paintTile(sf::Vector2f(worldx, worldy), newColor);
}

bool Canvas::paintTile(sf::Vector2f worldxy, sf::Color newColor)
{
	if (worldxy.x > 0 && worldxy.y > 0) {
		int tileX = static_cast<int>(worldxy.x / TILESIZE);
		int tileY = static_cast<int>(worldxy.y / TILESIZE);

		if (tileY < tileGrid.size() && tileX < tileGrid.at(tileY).size()) {
			tileGrid.at(tileY).at(tileX).changeColor(newColor);
			return true;
		}
	}
	return false;
}

bool Canvas::fogTile(sf::Vector2f worldxy)
{
	if (worldxy.x > 0 && worldxy.y > 0) {
		int tileX = static_cast<int>(worldxy.x / TILESIZE);
		int tileY = static_cast<int>(worldxy.y / TILESIZE);

		if (tileY < tileGrid.size() && tileX < tileGrid.at(tileY).size()) {
			tileGrid.at(tileY).at(tileX).setFog(true);
			return true;
		}
	}
	return false;
}

bool Canvas::unfogTile(sf::Vector2f worldxy)
{
	if (worldxy.x > 0 && worldxy.y > 0) {
		int tileX = static_cast<int>(worldxy.x / TILESIZE);
		int tileY = static_cast<int>(worldxy.y / TILESIZE);

		if (tileY < tileGrid.size() && tileX < tileGrid.at(tileY).size()) {
			tileGrid.at(tileY).at(tileX).setFog(false);
			return true;
		}
	}
	return false;
}

bool Canvas::isFogged(sf::Vector2i vectorPos) 
{
	unsigned int posX = static_cast<int>(vectorPos.x);
	unsigned int posY = static_cast<int>(vectorPos.y);

	if (posY < tileGrid.size() && posX < tileGrid.at(posY).size()) {
		if (tileGrid.at(posY).at(posX).getFog()) {
			return true;
		}
	}
	return false;
}

bool Canvas::eraseTile(sf::Vector2f worldxy)
{
	return paintTile(worldxy, defaultColor);
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
	reconstruct();
}

void Canvas::removeColumn(unsigned int locX){
	for (int y = 0; y < tileGrid.size(); y++) {
		tileGrid.at(y).erase(tileGrid.at(y).begin() + locX);
	}
	reconstruct();
}

void Canvas::addRowToBottom(bool shouldReconstruct){
	std::vector<Tile> newRow;
	for (int i = 0; i < tileGrid.at(0).size(); i++) {
		newRow.push_back(Tile(defaultColor));
		//std::cout << "Pushed new row\n";
	}
	tileGrid.push_back(newRow);
	if (shouldReconstruct)
		reconstruct();
}

void Canvas::addColumnToRight(bool shouldReconstruct){
	for (int i = 0; i < tileGrid.size(); i++) {
		tileGrid.at(i).push_back(Tile(defaultColor));
		int placeX = tileGrid.at(i).size() - 1;
		//std::cout << "Pushed new column\n";
	}
	if (shouldReconstruct)
		reconstruct();
}

void Canvas::draw(sf::RenderWindow& window) {
	

	window.draw(tileVertexes);
	window.draw(fogVertexes, &fogCloudTexture);
}


void Canvas::update(){

	int sizeY = tileGrid.size();
	int sizeX = tileGrid.at(0).size();

	for (int i = 0; i < sizeY; i++) {
		for (int w = 0; w < sizeX; w++) {

			if (tileGrid.at(i).at(w).shouldUpdate()) {
				sf::Vertex* tileQuad = &tileVertexes[(w + (i * tileGrid.at(i).size())) * 4];
				//std::cout << "Got the vertexes at: " << w + (i * tileGrid.at(i).size()) * 4 << std::endl;
				//std::cout << "Took the x value: " << w << " + " << i << " * " << tileGrid.at(i).size() << " all times 4\n";

				sf::Color newColor = tileGrid.at(i).at(w).getColor();

				tileQuad[0].color = newColor;
				tileQuad[1].color = newColor;
				tileQuad[2].color = newColor;
				tileQuad[3].color = newColor;



				sf::Vertex* fogQuad = &fogVertexes[(w + (i * tileGrid.at(i).size())) * 4];

				if (tileGrid.at(i).at(w).getFog()) {
					fogQuad[0].color = sf::Color::White;
					fogQuad[1].color = sf::Color::White;
					fogQuad[2].color = sf::Color::White;
					fogQuad[3].color = sf::Color::White;
				}
				else {
					fogQuad[0].color = sf::Color::Transparent;
					fogQuad[1].color = sf::Color::Transparent;
					fogQuad[2].color = sf::Color::Transparent;
					fogQuad[3].color = sf::Color::Transparent;
				}



				tileGrid.at(i).at(w).setUpdate(false);

			}

			

		}
	}


	
}

void Canvas::reconstruct(){

	//Tile Vertexes.
	sf::VertexArray newVertexes(sf::Quads);

	for (int i = 0; i < tileGrid.size(); i++) {
		for (int w = 0; w < tileGrid.at(i).size(); w++) {

			newVertexes.append(sf::Vertex(sf::Vector2f(w * 25, i * 25), tileGrid.at(i).at(w).getColor()));
			newVertexes.append(sf::Vertex(sf::Vector2f((w + 1) * 25, i * 25), tileGrid.at(i).at(w).getColor()));
			newVertexes.append(sf::Vertex(sf::Vector2f((w + 1) * 25, (i + 1) * 25), tileGrid.at(i).at(w).getColor()));
			newVertexes.append(sf::Vertex(sf::Vector2f(w * 25, (i + 1) * 25), tileGrid.at(i).at(w).getColor()));

		}
	}
	tileVertexes = newVertexes;


	//Fog Vertexes.
	newVertexes.clear();

	for (int i = 0; i < tileGrid.size(); i++) {
		for (int w = 0; w < tileGrid.at(i).size(); w++) {

			sf::Vertex topLeft(sf::Vertex(sf::Vector2f(w * 25, i * 25)));
			sf::Vertex topRight(sf::Vector2f((w + 1) * 25, i * 25));
			sf::Vertex bottomRight(sf::Vector2f((w + 1) * 25, (i + 1) * 25));
			sf::Vertex bottomLeft(sf::Vector2f(w * 25, (i + 1) * 25));



			topLeft.texCoords = sf::Vector2f(0, 0);
			topRight.texCoords = sf::Vector2f(25, 0);
			bottomRight.texCoords = sf::Vector2f(25, 25);
			bottomLeft.texCoords = sf::Vector2f(0, 25);
			if (!tileGrid.at(i).at(w).getFog()) {
				topLeft.color = sf::Color::Transparent;
				topRight.color = sf::Color::Transparent;
				bottomRight.color = sf::Color::Transparent;
				bottomLeft.color = sf::Color::Transparent;
			}

			newVertexes.append(topLeft);
			newVertexes.append(topRight);
			newVertexes.append(bottomRight);
			newVertexes.append(bottomLeft);

		}
	}

	fogVertexes = newVertexes;


	std::cout << "Reconstructed" << std::endl;

}
