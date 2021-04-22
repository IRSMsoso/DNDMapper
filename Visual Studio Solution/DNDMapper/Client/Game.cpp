#include "Game.h"

Game::Game(MenuInfo menuInfo, GameAction action, std::string filename) : Menu(menuInfo), ui(window, menuInfo.resourceManager), canvas(&camera, networkManager, resourceManager) {
	camera.move(10, 10);

	window->setView(camera);
	window->setKeyRepeatEnabled(false);


	//Setup Initial Variable Values
	zoomFactor = 1;

	//Load Cursors
	handCursor.loadFromSystem(sf::Cursor::Hand);
	defaultCursor.loadFromSystem(sf::Cursor::Arrow);
	//Set cursor to default to start
	window->setMouseCursor(defaultCursor);

	if (isDM)
		selectedTool = ToolType::paintingTool;
	else
		selectedTool = ToolType::tokenTool;

	mouseAction = MouseAction::none;
	previousAction = MouseAction::none;

	selectedColor = sf::Color::White;


	fpsText.setFillColor(sf::Color::Blue);
	fpsText.setFont(*resourceManager->getFontResource("arialfont"));


	gameIDAquired = false;

	if (action == GameAction::newGame || action == GameAction::loadGame) {
		isDM = true;
	}
	else {
		isDM = false;
	}

	//UI setup
	ui.setup(isDM);


	//Loading
	if (action == GameAction::loadGame) {
		DNDProto::Map map;
		std::fstream input(filename + ".cam", std::ios::in | std::ios::binary);
		if (!map.ParseFromIstream(&input)) {
			printf("Error, could not parse map from file %s", (filename + ".cam").c_str());
			close();
		}
		else {
			canvas.loadMap(map);
		}
		input.close();
	}
	if (isDM) {
		m_filename = filename;
	}
}

Game::~Game() {
	std::cout << "Deconstructed Game Object.\n";
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	

	target.draw(canvas);
	target.draw(ui);

	if (mouseAction == MouseAction::colorPicking) {
		target.draw(colorWheel);
	}
	
	target.draw(fpsText);

}


void Game::update(){
	//Debug
	//std::cout << "Should Game Close? " << getShouldClose();

	//FPS
	sf::Time frameTime = fpsClock.restart();
	float fps = 1.f / frameTime.asSeconds();
	fpsText.setString(std::to_string(fps));

	//Update from all networking commands.
	std::vector<DNDProto::NetworkMessage> inMessages;
	inMessages = networkManager->getMessagesOfType(DNDProto::NetworkMessage::MessageType::NetworkMessage_MessageType_Update);
	for (int i = 0; i < inMessages.size(); i++) {
		DNDProto::NetworkMessage message = inMessages.at(i);

		std::cout << "Resolving network message\n";

		if (message.has_tileupdate()) {
			DNDProto::TileUpdate tileUpdate = message.tileupdate();
			if (tileUpdate.has_newcolor())
				canvas.paintTile(tileUpdate.posx(), tileUpdate.posy(), sf::Color(tileUpdate.newcolor()), false);
			if (tileUpdate.has_newfogged())
				if (tileUpdate.newfogged())
					canvas.fogTile(sf::Vector2f(tileUpdate.posx(), tileUpdate.posy()), false);
				else
					canvas.unfogTile(sf::Vector2f(tileUpdate.posx(), tileUpdate.posy()), false);
		}

		if (message.has_tokenupdate()) {

			DNDProto::Token tokenUpdate = message.tokenupdate();

			if (tokenUpdate.has_isdestroy() && tokenUpdate.isdestroy()) {
				canvas.eraseToken(tokenUpdate.id());
			}
			else if (canvas.getTokenFromID(tokenUpdate.id()) == nullptr) {
				canvas.createToken(sf::Vector2f(tokenUpdate.posx(), tokenUpdate.posy()), sf::Color(tokenUpdate.color()), tokenUpdate.id(), false);
			}
			else {
				Token* change_token = canvas.getTokenFromID(tokenUpdate.id());
				if (tokenUpdate.has_name())
					change_token->setName(tokenUpdate.name());
				if (tokenUpdate.has_posx() && tokenUpdate.has_posy())
					change_token->setPosition(sf::Vector2f(tokenUpdate.posx(), tokenUpdate.posy()));
			}
		}

		if (!isDM && message.has_map()) {
			DNDProto::Map map = message.map();

			canvas.loadMap(map);

		}
	}


	//Panning Logic
	if (isPanning) {
		sf::Vector2f currentMouseLoc = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
		sf::Vector2f moveVector = panLockLoc - currentMouseLoc;
		camera.move(moveVector);
	}

	//Update the selected token (Mainly for blinking cursor logic).
	if (mouseAction == MouseAction::changingName && selectedToken != nullptr) {
		selectedToken->update();
	}

	//Camera moving with WASD Logic
	if (mouseAction != MouseAction::changingName) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			camera.move(0, -CAMERAMOVESPEED * frameTime.asSeconds());
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			camera.move(-CAMERAMOVESPEED * frameTime.asSeconds(), 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			camera.move(0, CAMERAMOVESPEED * frameTime.asSeconds());
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			camera.move(CAMERAMOVESPEED * frameTime.asSeconds(), 0);
		}


		//Mouse Action Logic
		if (mouseAction == MouseAction::painting) {
			canvas.paintTile(window->mapPixelToCoords(sf::Mouse::getPosition(*window)), selectedColor, true);
		}
		else if (mouseAction == MouseAction::erasing) {
			canvas.eraseTile(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
		}
		else if (mouseAction == MouseAction::fogging) {
			canvas.fogTile(window->mapPixelToCoords(sf::Mouse::getPosition(*window)), true);
		}
		else if (mouseAction == MouseAction::unfogging) {
			canvas.unfogTile(window->mapPixelToCoords(sf::Mouse::getPosition(*window)), true);
		}
		else if (mouseAction == MouseAction::tokenMoving) {
			selectedToken->setPosition(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
			//std::cout << "Selected Token ID: " << selectedToken->getID() << std::endl;
			//Networking.
			DNDProto::NetworkMessage message;
			message.set_messagetype(DNDProto::NetworkMessage::MessageType::NetworkMessage_MessageType_Update);
			DNDProto::Token* token = new DNDProto::Token;
			token->set_id(selectedToken->getID());
			token->set_posx(selectedToken->getPosition().x);
			token->set_posy(selectedToken->getPosition().y);
			message.set_allocated_tokenupdate(token);
			networkManager->sendMessage(message);
		}
		else if (mouseAction == MouseAction::tokenResizing) {
			sf::Vector2f currentMouse = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

			sf::Vector2i newSize = originalSize + sf::Vector2i((currentMouse - mouseOrigin) / TILESIZE);
			if (newSize.x > 0 && newSize.y > 0)
				selectedToken->setSize(newSize);


		}

	}

	//Update Canvas
	canvas.update();


	//Restrict the Camera
	restrictCamera();


	//Update UI
	ui.updateElementsPositions();
	ui.updateElementsScales(zoomFactor);
	ui.updateElementsAnimations(frameTime, selectedColor);



	std::vector<DNDProto::NetworkMessage> gameIDMessages = networkManager->getMessagesOfType(DNDProto::NetworkMessage::MessageType::NetworkMessage_MessageType_ID);
	if (gameIDMessages.size() > 0) {
		std::cout << "SUCCESS\n";
		sf::Uint16 gameID = gameIDMessages.at(0).gameid();
		window->setTitle("Dungeons and Dragons! Game ID: " + std::to_string(gameID));
	}

	//If disconnected, get outta there.
	if (!networkManager->getIsConnected()) {
		close();
	}

	//Saving
	if (saveClock.getElapsedTime() > sf::seconds(20) && isDM) {
		save(true);
	}
}

void Game::interpretEvent(sf::Event pollingEvent){

	//Switch Statement Logic.
	switch (pollingEvent.type) {


	case sf::Event::Closed:
		if (isDM)
			save(false);
		close();
		
		//Also send message that we are leaving the game.
		break;


	case sf::Event::MouseWheelScrolled:

		switch (mouseAction) {
		case MouseAction::colorPicking:
			if (pollingEvent.mouseWheelScroll.delta > 0) {
				colorWheel.changeMiddle(10);
			}
			if (pollingEvent.mouseWheelScroll.delta < 0) {
				colorWheel.changeMiddle(-10);
			}
			break;

		default:
			sf::Vector2f beforeMouseLoc = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
			if (pollingEvent.mouseWheelScroll.delta > 0 && zoomFactor * ZOOMSPEED >= MAXZOOM) {
				camera.zoom(ZOOMSPEED);
				zoomFactor *= ZOOMSPEED;
				//std::cout << "Zoom in" << std::endl;
			}
			if (pollingEvent.mouseWheelScroll.delta < 0 && zoomFactor / ZOOMSPEED <= MINZOOM) {
				camera.zoom(1 / ZOOMSPEED);
				zoomFactor /= ZOOMSPEED;
				//std::cout << "Zoom out" << std::endl;
			}

			window->setView(camera);

			sf::Vector2f afterMouseLoc = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
			sf::Vector2f moveVector = beforeMouseLoc - afterMouseLoc;
			camera.move(moveVector.x, moveVector.y);

			window->setView(camera);
		}
		break;


	case sf::Event::MouseButtonPressed:
	{ //Scope needed to prevent initialization error on mouseWindowLocation variable.
		sf::Vector2i mouseWindowLocation = sf::Mouse::getPosition(*window);

		switch (pollingEvent.mouseButton.button) {
		case sf::Mouse::Button::Middle:
			isPanning = true;
			panLockLoc = window->mapPixelToCoords(mouseWindowLocation);
			window->setMouseCursor(handCursor);
			break;

		case sf::Mouse::Button::Left:
			switch (mouseAction) {
			case MouseAction::none:
			{ //Scope needed to prevent initialization error on newTool variable.
				ToolType::ToolType newTool = ui.getToolClicked(mouseWindowLocation);

				if (newTool != ToolType::none) {
					selectedTool = newTool;
					std::cout << "Selected Tool: " << selectedTool << std::endl;
				}
				else {
					switch (selectedTool) {
					case ToolType::paintingTool:
						mouseAction = MouseAction::painting;
						break;

					case ToolType::fogTool:
						mouseAction = MouseAction::fogging;
						break;

					case ToolType::tokenTool:
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
							selectedToken = canvas.getClickedToken(window->mapPixelToCoords(mouseWindowLocation));
							if (selectedToken == nullptr) {
								canvas.createToken(window->mapPixelToCoords(mouseWindowLocation), selectedColor, true);
							}
							else {
								mouseAction = MouseAction::tokenResizing;
								mouseOrigin = window->mapPixelToCoords(mouseWindowLocation);
								originalSize = selectedToken->getSize();
							}

						}
						else {
							selectedToken = canvas.getClickedToken(window->mapPixelToCoords(mouseWindowLocation));
							if (selectedToken != nullptr)
								mouseAction = MouseAction::tokenMoving;
						}
						break;
					}
				}
			}
			break;

			case MouseAction::colorPicking:
			{
				std::cout << "Picked Color\n";
				sf::Texture screenshotTexture;
				screenshotTexture.create(window->getSize().x, window->getSize().y);
				screenshotTexture.update(*window);
				sf::Image screenshot = screenshotTexture.copyToImage();
				sf::Vector2i mouseLoc = sf::Mouse::getPosition(*window);
				selectedColor = screenshot.getPixel(mouseLoc.x, mouseLoc.y);

				mouseAction = MouseAction::none;
				break;
			}

			case MouseAction::changingName:
				selectedToken->setIsEditing(false);
				window->setKeyRepeatEnabled(false);
				selectedToken = nullptr;
				mouseAction = MouseAction::none;
				break;
			}
			break;

		case sf::Mouse::Button::Right:

			switch (mouseAction) {
			case MouseAction::none:
				switch (selectedTool) {
				case ToolType::paintingTool: //Painting tool selected + right click
					mouseAction = MouseAction::erasing;
					break;

				case ToolType::fogTool: //Fog tool selected + right click
					mouseAction = MouseAction::unfogging;
					break;

				case ToolType::tokenTool: //Token tool selected + right click
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) { //Shift Right Click
						canvas.eraseToken(window->mapPixelToCoords(mouseWindowLocation), true);
					}
					else { //Non shift Right Click
						selectedToken = canvas.getClickedToken(window->mapPixelToCoords(mouseWindowLocation));
						if (selectedToken != nullptr) {
							selectedToken->setIsEditing(true);
							window->setKeyRepeatEnabled(true);
							mouseAction = MouseAction::changingName;
						}
					}
					break;
				}
				break;
			}

			break;
		}
		break;
	}

	case sf::Event::MouseButtonReleased:

		switch (pollingEvent.mouseButton.button) {

		case sf::Mouse::Button::Middle:
			isPanning = false;
			window->setMouseCursor(defaultCursor);
			break;

		case sf::Mouse::Button::Left:
			switch (mouseAction) {
			case MouseAction::painting:
				mouseAction = MouseAction::none;
				canvas.expand();
				break;

			case MouseAction::fogging:
				mouseAction = MouseAction::none;
				canvas.expand();
				break;

			case MouseAction::tokenMoving:
				mouseAction = MouseAction::none;
				if (selectedToken != nullptr) {

					selectedToken->snap();

					//Networking the Snap Movement.
					DNDProto::NetworkMessage message;
					message.set_messagetype(DNDProto::NetworkMessage::MessageType::NetworkMessage_MessageType_Update);
					DNDProto::Token* token = new DNDProto::Token;
					token->set_id(selectedToken->getID());
					token->set_posx(selectedToken->getPosition().x);
					token->set_posy(selectedToken->getPosition().y);
					message.set_allocated_tokenupdate(token);
					networkManager->sendMessage(message);

					selectedToken = nullptr;
				}
				break;

			case MouseAction::tokenResizing:
				selectedToken->snap();
				selectedToken = nullptr;
				mouseAction = MouseAction::none;
				break;
			}
			break;

		case sf::Mouse::Button::Right:
			switch (mouseAction) {
			case MouseAction::erasing:
				mouseAction = MouseAction::none;
				canvas.expand();
				break;

			case MouseAction::unfogging:
				mouseAction = MouseAction::none;
				canvas.expand();
				break;
			}
		}
		break;


	case sf::Event::KeyPressed:

		if (mouseAction == MouseAction::changingName) {

			int newKey = -1;
			std::cout << "Size: " << (sizeof(ALLOWEDKEYS) / sizeof(*ALLOWEDKEYS)) << std::endl;
			for (int i = 0; i < (sizeof(ALLOWEDKEYS) / sizeof(*ALLOWEDKEYS)); i++) {
				if (ALLOWEDKEYS[i] == pollingEvent.key.code)
					newKey = i;
			}
			if (newKey != -1) {
				if (pollingEvent.key.shift) { //Shift is pressed, add upper case letter.
					selectedToken->addNameLetter(UPPERCASEALPHABET[newKey]);
				}
				else { //Shift isn't pressed, add lower case letter.
					selectedToken->addNameLetter(LOWERCASEALPHABET[newKey]);
				}
				//Networking
				DNDProto::NetworkMessage message;
				message.set_messagetype(DNDProto::NetworkMessage::MessageType::NetworkMessage_MessageType_Update);
				DNDProto::Token* token = new DNDProto::Token;
				token->set_id(selectedToken->getID());
				token->set_name(selectedToken->getName());
				message.set_allocated_tokenupdate(token);
				networkManager->sendMessage(message);
			}
			else if (pollingEvent.key.code == sf::Keyboard::Key::Backspace) {
				selectedToken->removeNameLetter();
				//Networking
				DNDProto::NetworkMessage message;
				message.set_messagetype(DNDProto::NetworkMessage::MessageType::NetworkMessage_MessageType_Update);
				DNDProto::Token* token = new DNDProto::Token;
				token->set_id(selectedToken->getID());
				token->set_name(selectedToken->getName());
				message.set_allocated_tokenupdate(token);
				networkManager->sendMessage(message);
			}
			else if (pollingEvent.key.code == sf::Keyboard::Key::Enter) {
				selectedToken->setIsEditing(false);
				window->setKeyRepeatEnabled(false);
				selectedToken = nullptr;
				mouseAction = MouseAction::none;
			}

		}
		else {
			switch (pollingEvent.key.code) {
			case sf::Keyboard::E:
				if (mouseAction == MouseAction::none)
					previousAction = mouseAction;
				mouseAction = MouseAction::colorPicking;
				colorWheel.setPosition(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
				break;

			}
		}

		break;



	case sf::Event::KeyReleased:

		switch (pollingEvent.key.code) {
		case sf::Keyboard::E:
			if (mouseAction == MouseAction::colorPicking) {
				mouseAction = previousAction;
				previousAction = MouseAction::none;
			}
			break;
		}
		break;

	}

}

void Game::save(bool send) {
	DNDProto::Map map;
	canvas.saveMap(map);

	std::fstream output(m_filename + ".cam", std::ios::out | std::ios::trunc | std::ios::binary);
	if (!map.SerializeToOstream(&output)) {
		printf("Failed to write map to file &s", (m_filename + ".cam").c_str());
	}
	//Also send it to network

	if (send) {
		DNDProto::NetworkMessage message;
		message.set_messagetype(DNDProto::NetworkMessage::MessageType::NetworkMessage_MessageType_Update);
		DNDProto::Map* map = new DNDProto::Map;
		canvas.saveMap(*map);
		message.set_allocated_map(map);
		networkManager->sendMessage(message);
	}


	saveClock.restart();
}

void Game::restrictCamera(){
	window->setView(camera);
	if (window->mapPixelToCoords(sf::Vector2i(0, 0)).x < 1) {
		camera.move(1 - window->mapPixelToCoords(sf::Vector2i(0, 0)).x, 0);
	}
	if (window->mapPixelToCoords(sf::Vector2i(0, 0)).y < 1) {
		camera.move(0, 1 - window->mapPixelToCoords(sf::Vector2i(0, 0)).y);
	}
	if (window->mapPixelToCoords(sf::Vector2i(camera.getSize() / zoomFactor)).x > canvas.getTileGrid()->at(0).size() * 25.f - 1) {
		camera.move(canvas.getTileGrid()->at(0).size() * 25.f - 1 - window->mapPixelToCoords(sf::Vector2i(camera.getSize() / zoomFactor)).x, 0);
	}
	if (window->mapPixelToCoords(sf::Vector2i(camera.getSize() / zoomFactor)).y > canvas.getTileGrid()->size() * 25.f - 1) {
		camera.move(0, canvas.getTileGrid()->size() * 25.f - 1 - window->mapPixelToCoords(sf::Vector2i(camera.getSize() / zoomFactor)).y);
	}
	window->setView(camera);
}
