#include "Canvas.h"

Canvas::Canvas(sf::View* newView, NetworkManager* newNetworkManager, ResourceManager* newResourceManager){
	sf::Clock setupClock;

	networkManager = newNetworkManager;
	resourceManager = newResourceManager;

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

	setHiddenFog(true);

	std::cout << "Canvas setup took " << setupClock.getElapsedTime().asSeconds() << " seconds.\n";
}

Canvas::~Canvas(){

}

//Same call as other paintTile function
bool Canvas::paintTile(float worldx, float worldy, sf::Color newColor, bool shouldSend) {
	return paintTile(sf::Vector2f(worldx, worldy), newColor, shouldSend);
}

bool Canvas::paintTile(sf::Vector2f worldxy, sf::Color newColor, bool shouldSend) {
	if (worldxy.x > 0 && worldxy.y > 0) {
		int tileX = static_cast<int>(worldxy.x / TILESIZE);
		int tileY = static_cast<int>(worldxy.y / TILESIZE);

		if (tileY < tileGrid.size() && tileX < tileGrid.at(tileY).size()) {
			if (tileGrid.at(tileY).at(tileX).getColor() != newColor) {
				//Client Stuff.
				tileGrid.at(tileY).at(tileX).changeColor(newColor);
				updateQueue.push_back(sf::Vector2i(tileX, tileY));
				//std::cout << "Painted Tile";
				//Network Stuff.
				if (shouldSend) {
					DNDProto::NetworkMessage message;
					message.set_messagetype(DNDProto::NetworkMessage::MessageType::NetworkMessage_MessageType_Update);
					DNDProto::TileUpdate* tileUpdate = new DNDProto::TileUpdate;
					tileUpdate->set_posx(worldxy.x);
					tileUpdate->set_posy(worldxy.y);
					tileUpdate->set_newcolor(newColor.toInteger());
					message.set_allocated_tileupdate(tileUpdate);
					networkManager->sendMessage(message);
				}
				return true;
			}
		}
	}
	return false;
}

bool Canvas::fogTile(sf::Vector2f worldxy, bool shouldSend) {
	if (worldxy.x > 0 && worldxy.y > 0) {
		int tileX = static_cast<int>(worldxy.x / TILESIZE);
		int tileY = static_cast<int>(worldxy.y / TILESIZE);

		if (tileY < tileGrid.size() && tileX < tileGrid.at(tileY).size()) {
			if (!isFogged(sf::Vector2i(tileX, tileY))) {
				//Client Stuff.
				tileGrid.at(tileY).at(tileX).setFog(true);
				updateQueue.push_back(sf::Vector2i(tileX, tileY));
				//Network Stuff.
				if (shouldSend) {
					DNDProto::NetworkMessage message;
					message.set_messagetype(DNDProto::NetworkMessage::MessageType::NetworkMessage_MessageType_Update);
					DNDProto::TileUpdate* tileUpdate = new DNDProto::TileUpdate;
					tileUpdate->set_posx(worldxy.x);
					tileUpdate->set_posy(worldxy.y);
					tileUpdate->set_newfogged(true);
					message.set_allocated_tileupdate(tileUpdate);
					networkManager->sendMessage(message);
				}
				return true;
			}
		}
	}
	return false;
}

bool Canvas::unfogTile(sf::Vector2f worldxy, bool shouldSend) {
	if (worldxy.x > 0 && worldxy.y > 0) {
		int tileX = static_cast<int>(worldxy.x / TILESIZE);
		int tileY = static_cast<int>(worldxy.y / TILESIZE);

		if (tileY < tileGrid.size() && tileX < tileGrid.at(tileY).size()) {
			if (isFogged(sf::Vector2i(tileX, tileY))) {
				//Client Stuff.
				tileGrid.at(tileY).at(tileX).setFog(false);
				updateQueue.push_back(sf::Vector2i(tileX, tileY));
				//Network Stuff.
				if (shouldSend) {
					DNDProto::NetworkMessage message;
					message.set_messagetype(DNDProto::NetworkMessage::MessageType::NetworkMessage_MessageType_Update);
					DNDProto::TileUpdate* tileUpdate = new DNDProto::TileUpdate;
					tileUpdate->set_posx(worldxy.y);
					tileUpdate->set_posy(worldxy.y);
					tileUpdate->set_newfogged(false);
					message.set_allocated_tileupdate(tileUpdate);
					networkManager->sendMessage(message);
				}
				return true;
			}
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

bool Canvas::createToken(sf::Vector2f worldxy, sf::Color newColor, bool shouldSend){

	sf::Uint16 newID = 0;
	for (int i = 0; i < tokenList.size(); i++) {
		if (tokenList.at(i).getID() == newID) {
			newID += 1;
			i = -1;
		}
	}

	return createToken(worldxy, newColor, newID, shouldSend);
}

bool Canvas::createToken(sf::Vector2f worldxy, sf::Color newColor, sf::Uint16 newID, bool shouldSend)
{
	if (worldxy.x >= 0 && worldxy.y >= 0) {
		int tileX = static_cast<int>(worldxy.x / TILESIZE);
		int tileY = static_cast<int>(worldxy.y / TILESIZE);

		if (tileY < tileGrid.size() && tileX < tileGrid.at(tileY).size()) {

			//Client Stuff
			Token newToken(newColor, sf::Vector2f(tileX * TILESIZE, tileY * TILESIZE) + sf::Vector2f(TILESIZE / 2.f, TILESIZE / 2.f), *resourceManager->getFontResource("arialfont"), newID);
			tokenList.push_back(newToken);
			std::cout << "Created Token\n";

			//Network Stuff.
			if (shouldSend) {
				DNDProto::NetworkMessage message;
				message.set_messagetype(DNDProto::NetworkMessage::MessageType::NetworkMessage_MessageType_Update);
				DNDProto::Token* token = new DNDProto::Token;
				token->set_posx(worldxy.x);
				token->set_posy(worldxy.y);
				token->set_name(newToken.getName());
				token->set_id(newToken.getID());
				token->set_color(newToken.getColor().toInteger());
				message.set_allocated_tokenupdate(token);
				networkManager->sendMessage(message);
			}
			return true;
		}
	}

	return false;
}

bool Canvas::eraseToken(sf::Vector2f worldxy, bool shouldSend)
{

	for (int i = tokenList.size() - 1; i >= 0; i--) {
		if (tokenList.at(i).isClicked(worldxy)) {
			//Network Stuff.
			if (shouldSend) {
				DNDProto::NetworkMessage message;
				message.set_messagetype(DNDProto::NetworkMessage::MessageType::NetworkMessage_MessageType_Update);
				DNDProto::Token* token = new DNDProto::Token;
				token->set_id(tokenList.at(i).getID());
				token->set_isdestroy(true);
				message.set_allocated_tokenupdate(token);
				networkManager->sendMessage(message);
			}
			//Client Stuff last because it deletes it.
			tokenList.erase(tokenList.begin() + i);
			return true;
		}
	}

	return false;
}

bool Canvas::eraseToken(sf::Uint16 id) {

	for (int i = 0; i < tokenList.size(); i++) {
		if (tokenList.at(i).getID() == id) {
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

Token * Canvas::getTokenFromID(sf::Uint16 id) {

	for (int i = 0; i < tokenList.size(); i++) {
		if (tokenList.at(i).getID() == id)
			return &tokenList.at(i);
	}

	return nullptr;
}

bool Canvas::eraseTile(sf::Vector2f worldxy) {
	return paintTile(worldxy, defaultColor, true);
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
		changed = true;
	}
	
	while ((maxy < tileGrid.size() - EXPANDDISTANCE - 1 || maxy == -1) && tileGrid.size() > MINSIZE.y) {
		removeRow(tileGrid.size() - 1);
		changed = true;
	}
	
	


	if (changed) {
		reconstruct();
		std::cout << "Size: " << tileGrid.at(0).size() << ", " << tileGrid.size() << std::endl;
	}



	return false;
}

void Canvas::saveMap(DNDProto::Map& map) {
	//sizeX
	map.set_sizex(tileGrid.at(0).size());

	//sizeY
	map.set_sizey(tileGrid.size());

	//tiles
	for (int y = 0; y < tileGrid.size(); y++) {
		for (int x = 0; x < tileGrid.at(y).size(); x++) {
			map.add_tiles(tileGrid.at(y).at(x).getColor().toInteger());
		}
	}

	//fogged
	for (int y = 0; y < tileGrid.size(); y++) {
		for (int x = 0; x < tileGrid.at(y).size(); x++) {
			map.add_fogged(tileGrid.at(y).at(x).getFog());
		}
	}

	//tokens
	for (int i = 0; i < tokenList.size(); i++) {
		DNDProto::Token* token = map.add_tokens();

		//name
		token->set_name(tokenList.at(i).getName());

		//color
		token->set_color(tokenList.at(i).getColor().toInteger());

		//posX
		token->set_posx(tokenList.at(i).getPosition().x);

		//posY
		token->set_posy(tokenList.at(i).getPosition().y);

		//ID
		token->set_id(tokenList.at(i).getID());

	}
}

bool Canvas::loadMap(DNDProto::Map& map) {

	//Checks.
	if (map.fogged_size() != map.tiles_size()) {
		return false;
	}

	//tiles
	tileGrid.clear();
	tokenList.clear();

	printf("SizeX: %u\n", map.sizex());
	printf("SizeX: %u\n", map.sizex());
	printf("SizeTiles: %u\n", map.tiles_size());
	printf("SizeFogs: %u\n", map.fogged_size());
	
	int i = 0;
	for (int y = 0; y < map.sizey(); y++) {
		std::vector<Tile> newRow;
		for (int x = 0; x < map.sizex(); x++) {
			if (i < map.tiles_size() && i < map.fogged_size()) {
				Tile tile(sf::Color(map.tiles(i)));
				tile.setFog(map.fogged(i));
				newRow.push_back(tile);
			}
			else {
				newRow.push_back(Tile(defaultColor));
			}
			i++;
		}
		tileGrid.push_back(newRow);
	}

	//tokens
	for (int i = 0; i < map.tokens_size(); i++) {
		DNDProto::Token tokenMessage = map.tokens(i);
		Token token(sf::Color(tokenMessage.color()), sf::Vector2f(tokenMessage.posx(), tokenMessage.posy()), *resourceManager->getFontResource("arialfont"), tokenMessage.id());
		token.setName(tokenMessage.name());
		token.updateName();
		token.updateNameLocation();
		tokenList.push_back(token);
	}

	reconstruct(); //Gotta make all those changes real. Fo sho.


	return true;
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
		if (!drawHiddenFog)
			target.draw(tokenList.at(i));
		else if (!tileGrid.at(tokenList.at(i).getPosition().y / TILESIZE).at(tokenList.at(i).getPosition().x / TILESIZE).getFog()){
			target.draw(tokenList.at(i));
		}
		//numDrawn += 1;
	}
	//std::cout << numDrawn << " tokens drawn\n";

	if (drawHiddenFog)
		target.draw(fogVertexes, resourceManager->getTextureResource("fogtilehiddentexture"));
	else
		target.draw(fogVertexes, resourceManager->getTextureResource("fogtiletexture"));
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
