#include "Canvas.h"

Canvas::Canvas(sf::View* newView){
	sf::Clock setupClock;

	//Set camera pointer.
	camera = newView;

	std::vector<Tile> firstRow;
	firstRow.push_back(Tile(defaultColor));
	tileGrid.push_back(firstRow);

	for (int i = 1; i < MINSIZE.x; i++) {
		addColumnToRight(false);
	}
	for (int i = 1; i < MINSIZE.y; i++) {
		addRowToBottom(false);
	}
	reconstruct();



	fogCloudTexture.loadFromFile("FogCloud.png");

	tokenFont.loadFromFile("arial.TTF");

	std::cout << "Canvas setup took " << setupClock.getElapsedTime().asSeconds() << " seconds.\n";
}

Canvas::~Canvas(){

}

//Same call as other paintTile function
bool Canvas::paintTile(float worldx, float worldy, sf::Color newColor) {
	return paintTile(sf::Vector2f(worldx, worldy), newColor);
}

bool Canvas::paintTile(sf::Vector2f worldxy, sf::Color newColor) {
	if (worldxy.x > 0 && worldxy.y > 0) {
		int tileX = static_cast<int>(worldxy.x / TILESIZE);
		int tileY = static_cast<int>(worldxy.y / TILESIZE);

		if (tileY < tileGrid.size() && tileX < tileGrid.at(tileY).size()) {
			tileGrid.at(tileY).at(tileX).changeColor(newColor);
			updateQueue.push_back(sf::Vector2i(tileX, tileY));
			return true;
		}
	}
	return false;
}

bool Canvas::fogTile(sf::Vector2f worldxy) {
	if (worldxy.x > 0 && worldxy.y > 0) {
		int tileX = static_cast<int>(worldxy.x / TILESIZE);
		int tileY = static_cast<int>(worldxy.y / TILESIZE);

		if (tileY < tileGrid.size() && tileX < tileGrid.at(tileY).size()) {
			tileGrid.at(tileY).at(tileX).setFog(true);
			updateQueue.push_back(sf::Vector2i(tileX, tileY));
			return true;
		}
	}
	return false;
}

bool Canvas::unfogTile(sf::Vector2f worldxy) {
	if (worldxy.x > 0 && worldxy.y > 0) {
		int tileX = static_cast<int>(worldxy.x / TILESIZE);
		int tileY = static_cast<int>(worldxy.y / TILESIZE);

		if (tileY < tileGrid.size() && tileX < tileGrid.at(tileY).size()) {
			tileGrid.at(tileY).at(tileX).setFog(false);
			updateQueue.push_back(sf::Vector2i(tileX, tileY));
			return true;
		}
	}
	return false;
}

bool Canvas::isFogged(sf::Vector2i vectorPos) {
	unsigned int posX = static_cast<int>(vectorPos.x);
	unsigned int posY = static_cast<int>(vectorPos.y);

	if (posY < tileGrid.size() && posX < tileGrid.at(posY).size()) {
		if (tileGrid.at(posY).at(posX).getFog()) {
			return true;
		}
	}
	return false;
}

bool Canvas::createToken(sf::Vector2f worldxy, sf::Color newColor){

	if (worldxy.x >= 0 && worldxy.y >= 0) {
		int tileX = static_cast<int>(worldxy.x / TILESIZE);
		int tileY = static_cast<int>(worldxy.y / TILESIZE);

		if (tileY < tileGrid.size() && tileX < tileGrid.at(tileY).size()) {
			
			sf::Uint16 newID = 0;
			for (int i = 0; i < tokenList.size(); i++) {
				if (tokenList.at(i).getID() == newID) {
					newID += 1;
					i = -1;
				}
			}

			Token newToken(newColor, sf::Vector2f(tileX * TILESIZE, tileY * TILESIZE) + sf::Vector2f(TILESIZE / 2.f, TILESIZE / 2.f), tokenFont, newID);
			tokenList.push_back(newToken);

			std::cout << "Created Token\n";
			return true;
		}
	}

	return false;
}

bool Canvas::eraseToken(sf::Vector2f worldxy)
{

	for (int i = tokenList.size() - 1; i >= 0; i--) {
		if (tokenList.at(i).isClicked(worldxy)) {
			tokenList.erase(tokenList.begin() + i);
			return true;
		}
	}

	return false;
}

Token * Canvas::getClickedToken(sf::Vector2f worldxy)
{
	for (int i = tokenList.size() - 1; i >= 0; i--) {
		if (tokenList.at(i).isClicked(worldxy)) {
			return &tokenList.at(i);
		}
	}
	std::cout << "Couldn't find token\n";
	return nullptr;
}

bool Canvas::eraseTile(sf::Vector2f worldxy) {
	return paintTile(worldxy, defaultColor);
}

std::vector<std::vector<Tile>>* Canvas::getTileGrid(){
	return &tileGrid;
}

//Helper Function. Expands the canvas to accommodate out of bounds clicks.
bool Canvas::expand()
{
	int minx = -1;
	int miny = -1;
	int maxx = -1;
	int maxy = -1;
	int gridsizex = tileGrid.at(0).size();
	int gridsizey = tileGrid.size();

	for (int y = 0; y < gridsizey; y++) {
		for (int x = 0; x < gridsizex; x++) {

			if (tileGrid.at(y).at(x).getColor() != defaultColor) {

				if (minx == -1) {
					minx = x;
					maxx = x;
					miny = y;
					maxy = y;
				}
				else {
					if (x > maxx)
						maxx = x;
					if (x < minx)
						minx = x;
					if (y > maxy)
						maxy = y;
					if (y < miny)
						miny = y;
				}

			}
		}
	}

	bool changed = false;

	if (minx != -1) {

		int rounds = 0;

		if (minx < EXPANDDISTANCE) {
			for (int i = 0; i < (EXPANDDISTANCE - minx); i++) {
				addColumnToLeft(false);
				rounds++;
			}
			maxx += rounds;
			rounds = 0;
			gridsizex = tileGrid.at(0).size();
			changed = true;
		}

		if (miny < EXPANDDISTANCE) {
			for (int i = 0; i < (EXPANDDISTANCE - miny); i++) {
				addRowToTop(false);
				rounds++;
			}
			maxy += rounds;
			rounds = 0;
			gridsizey = tileGrid.size();
			changed = true;
		}

		if (maxx > (gridsizex - EXPANDDISTANCE - 1)) {
			std::cout << (maxx - (gridsizex - EXPANDDISTANCE - 1));

			for (int i = 0; i < (maxx - (gridsizex - EXPANDDISTANCE - 1)); i++) {
				addColumnToRight(false);
			}
			gridsizex = tileGrid.at(0).size();
			changed = true;
		}

		if (maxy > (gridsizey - EXPANDDISTANCE - 1)) {

			for (int i = 0; i < (maxy - (gridsizey - EXPANDDISTANCE - 1)); i++) {
				addRowToBottom(false);
			}
			gridsizey = tileGrid.size();
			changed = true;
		}


	}


	while ((minx > EXPANDDISTANCE || minx == -1) && tileGrid.at(0).size() > MINSIZE.x) {
		removeColumn(0);
		changed = true;
	}
	while ((miny > EXPANDDISTANCE || miny == -1) && tileGrid.size() > MINSIZE.y) {
		removeRow(0);
		changed = true;
	}
	while ((maxx < tileGrid.at(0).size() - EXPANDDISTANCE - 1 || maxx == -1) && tileGrid.at(0).size() > MINSIZE.x) {
		removeColumn(tileGrid.at(0).size() - 1);
	}
	
	while ((maxy < tileGrid.size() - EXPANDDISTANCE - 1 || maxy == -1) && tileGrid.size() > MINSIZE.y) {
		removeRow(tileGrid.size() - 1);
	}
	
	


	if (changed) {
		reconstruct();
		std::cout << "Size: " << tileGrid.at(0).size() << ", " << tileGrid.size() << std::endl;
	}



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

void Canvas::updateBead(int x, int y, int index){
	sf::Vertex* point = &beadVertexes[index];


	sf::Color TL = tileGrid.at(y - 1).at(x - 1).getColor();
	sf::Color TR = tileGrid.at(y - 1).at(x).getColor();
	sf::Color BL = tileGrid.at(y).at(x - 1).getColor();
	sf::Color BR = tileGrid.at(y).at(x).getColor();


	float avgR = ((float)TL.r + (float)TR.r + (float)BL.r + (float)BR.r) / 4.f;
	float avgG = ((float)TL.g + (float)TR.g + (float)BL.g + (float)BR.g) / 4.f;
	float avgB = ((float)TL.b + (float)TR.b + (float)BL.b + (float)BR.b) / 4.f;

	double rgbcircle = abs(((0.299 * avgR + 0.587 * avgG + 0.114 * avgB) / 255) - 1) * 255;

	point->color = sf::Color(rgbcircle, rgbcircle, rgbcircle, 255);


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
		//std::cout << "Pushed new column\n";
	}
	if (shouldReconstruct)
		reconstruct();
}

void Canvas::addRowToTop(bool shouldReconstruct){
	std::vector<Tile> newRow;
	for (int i = 0; i < tileGrid.at(0).size(); i++) {
		newRow.push_back(Tile(defaultColor));
	}

	tileGrid.insert(tileGrid.begin(), newRow);

	for (int i = 0; i < tokenList.size(); i++) {
		tokenList.at(i).setPosition(tokenList.at(i).getPosition() + sf::Vector2f(0.f, 25.f));
	}

	camera->move(sf::Vector2f(0.f, 25.f));

	if (shouldReconstruct)
		reconstruct();
}

void Canvas::addColumnToLeft(bool shouldReconstruct){
	for (int i = 0; i < tileGrid.size(); i++) {
		tileGrid.at(i).insert(tileGrid.at(i).begin(), Tile(defaultColor));
	}

	for (int i = 0; i < tokenList.size(); i++) {
		tokenList.at(i).setPosition(tokenList.at(i).getPosition() + sf::Vector2f(25.f, 0.f));
	}

	camera->move(sf::Vector2f(25.f, 0.f));

	if (shouldReconstruct)
		reconstruct();
}


void Canvas::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	

	target.draw(tileVertexes);
	target.draw(beadVertexes);

	//int numDrawn = 0;
	for (int i = 0; i < tokenList.size(); i++) {
		target.draw(tokenList.at(i));
		//numDrawn += 1;
	}
	//std::cout << numDrawn << " tokens drawn\n";

	target.draw(fogVertexes, &fogCloudTexture);
}


void Canvas::update(){

	int sizeY = tileGrid.size();
	int sizeX = tileGrid.at(0).size();

	while(updateQueue.size() > 0){

		int x = updateQueue.at(0).x;
		int y = updateQueue.at(0).y;

		sf::Vertex* tileQuad = &tileVertexes[(x + (y * tileGrid.at(y).size())) * 4];
		//std::cout << "Got the vertexes at: " << w + (i * tileGrid.at(i).size()) * 4 << std::endl;
		//std::cout << "Took the x value: " << w << " + " << i << " * " << tileGrid.at(i).size() << " all times 4\n";

		sf::Color newColor = tileGrid.at(y).at(x).getColor();

		tileQuad[0].color = newColor;
		tileQuad[1].color = newColor;
		tileQuad[2].color = newColor;
		tileQuad[3].color = newColor;



		sf::Vertex* fogQuad = &fogVertexes[(x + (y * tileGrid.at(y).size())) * 4];

		if (tileGrid.at(y).at(x).getFog()) {
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


		if (x < tileGrid.at(y).size() - 1 && y < tileGrid.size() - 1) {
			updateBead(x + 1, y + 1, (tileGrid.at(y).size() - 1) * y + x);
		}
		if (x < tileGrid.at(y).size() - 1 && y > 0) {
			updateBead(x + 1, y, (tileGrid.at(y).size() - 1) * (y - 1) + x);
		}
		if (x > 0 && y < tileGrid.size() - 1) {
			updateBead(x, y + 1, (tileGrid.at(y).size() - 1) * (y) + (x - 1));
		}
		if (x > 0 && y > 0) {
			updateBead(x, y, (tileGrid.at(y).size() - 1) * (y - 1) + (x - 1));
		}


		updateQueue.erase(updateQueue.begin());
	}
}

void Canvas::reconstruct(){

	//Tile Vertexes.
	sf::VertexArray newVertexes(sf::Quads);

	for (int i = 0; i < tileGrid.size(); i++) {
		for (int w = 0; w < tileGrid.at(i).size(); w++) {

			newVertexes.append(sf::Vertex(sf::Vector2f(w * TILESIZE, i * TILESIZE), tileGrid.at(i).at(w).getColor()));
			newVertexes.append(sf::Vertex(sf::Vector2f((w + 1) * TILESIZE, i * TILESIZE), tileGrid.at(i).at(w).getColor()));
			newVertexes.append(sf::Vertex(sf::Vector2f((w + 1) * TILESIZE, (i + 1) * TILESIZE), tileGrid.at(i).at(w).getColor()));
			newVertexes.append(sf::Vertex(sf::Vector2f(w * TILESIZE, (i + 1) * TILESIZE), tileGrid.at(i).at(w).getColor()));

		}
	}
	tileVertexes = newVertexes;


	//Fog Vertexes.
	newVertexes.clear();

	for (int i = 0; i < tileGrid.size(); i++) {
		for (int w = 0; w < tileGrid.at(i).size(); w++) {

			sf::Vertex topLeft(sf::Vertex(sf::Vector2f(w * TILESIZE, i * TILESIZE)));
			sf::Vertex topRight(sf::Vector2f((w + 1) * TILESIZE, i * TILESIZE));
			sf::Vertex bottomRight(sf::Vector2f((w + 1) * TILESIZE, (i + 1) * TILESIZE));
			sf::Vertex bottomLeft(sf::Vector2f(w * TILESIZE, (i + 1) * TILESIZE));



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



	newVertexes.clear();
	newVertexes.setPrimitiveType(sf::Points);

	for (int y = 1; y < tileGrid.size(); y++) {
		for (int x = 1; x < tileGrid.at(y).size(); x++) {

			sf::Vertex newPoint;

			newPoint.position = sf::Vector2f(x * TILESIZE, y * TILESIZE);
			
			sf::Color TL = tileGrid.at(y - 1).at(x - 1).getColor();
			sf::Color TR = tileGrid.at(y - 1).at(x).getColor();
			sf::Color BL = tileGrid.at(y).at(x - 1).getColor();
			sf::Color BR = tileGrid.at(y).at(x).getColor();


			float avgR = ((float)TL.r + (float)TR.r + (float)BL.r + (float)BR.r) / 4.f;
			float avgG = ((float)TL.g + (float)TR.g + (float)BL.g + (float)BR.g) / 4.f;
			float avgB = ((float)TL.b + (float)TR.b + (float)BL.b + (float)BR.b) / 4.f;

			double rgbcircle = abs(((0.299 * avgR + 0.587 * avgG + 0.114 * avgB) / 255) - 1) * 255;

			newPoint.color = sf::Color(rgbcircle, rgbcircle, rgbcircle, 255);

			newVertexes.append(newPoint);

		}
	}

	beadVertexes = newVertexes;


	std::cout << "Reconstructed" << std::endl;

}
