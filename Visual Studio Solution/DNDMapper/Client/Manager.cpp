#include "Manager.h"

//Construct the Manager with the specific window settings.
Manager::Manager(sf::ContextSettings settings): window(sf::VideoMode(WINDOWX, WINDOWY), "Dungeons and Dragons!", sf::Style::Default, settings), camera(sf::FloatRect(0.f, 0.f, WINDOWX, WINDOWY)), ui(&window), canvas(&camera){

	camera.move(10, 10);

	window.setView(camera);
	window.setKeyRepeatEnabled(false);
	

	//Setup Initial Variable Values
	zoomFactor = 1;

	//Load Cursors
	handCursor.loadFromSystem(sf::Cursor::Hand);
	defaultCursor.loadFromSystem(sf::Cursor::Arrow);
	//Set cursor to default to start
	window.setMouseCursor(defaultCursor);

	selectedTool = ToolType::paintingTool;

	mouseAction = MouseAction::none;
	previousAction = MouseAction::none;
  
	selectedColor = sf::Color::White;
	
  
	fpsText.setFillColor(sf::Color::Blue);
	algerFont.loadFromFile("ALGER.TTF");
	fpsText.setFont(algerFont);


	//window.setFramerateLimit(60);
}

Manager::~Manager(){

}

//The main event loop of the Manager object.
void Manager::mainLoop(){

	//FPS
	sf::Clock fpsClock;

	//Testing Token Threshhold.
	//int threshhold = 0;

	//LOOP
	while (window.isOpen()) {


		//FPS
		sf::Time frameTime = fpsClock.restart();
		float fps = 1.f / frameTime.asSeconds();
		fpsText.setString(std::to_string(fps));

		/*
		//Testing Token Threshhold.
		if (fps > 80) {
			canvas.createToken(sf::Vector2f(0, 0), sf::Color::White);
			threshhold += 1;
			std::cout << threshhold << std::endl;
		}
		*/

		//Interpret each event in queue from main thread.
		sf::Event pollingEvent;
		while (window.pollEvent(pollingEvent)) {
			interpretEvent(pollingEvent);
		}

		//Panning Logic
		if (isPanning) {
			sf::Vector2f currentMouseLoc = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			sf::Vector2f moveVector = panLockLoc - currentMouseLoc;
			if (window.mapPixelToCoords(sf::Vector2i(0, 0)).x + moveVector.x > 1 && window.mapPixelToCoords(sf::Vector2i(camera.getSize() / zoomFactor)).x + moveVector.x < canvas.getTileGrid()->at(0).size() * 25.f - 1) {
				camera.move(moveVector.x, 0);
			}

			if (window.mapPixelToCoords(sf::Vector2i(0, 0)).y + moveVector.y > 1 && window.mapPixelToCoords(sf::Vector2i(camera.getSize() / zoomFactor)).y + moveVector.y < canvas.getTileGrid()->size() * 25.f - 1) {
				camera.move(0, moveVector.y);
			}
			window.setView(camera);
		}

		//Update the selected token (Mainly for blinking cursor logic).
		if (mouseAction == MouseAction::changingName && selectedToken != nullptr) {
			selectedToken->update();
		}


		//Camera moving with WASD Logic
		if (mouseAction != MouseAction::changingName) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
				if(window.mapPixelToCoords(sf::Vector2i(0, 0)).y - CAMERAMOVESPEED * frameTime.asSeconds() > 1)
					camera.move(0, -CAMERAMOVESPEED * frameTime.asSeconds());
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
				if (window.mapPixelToCoords(sf::Vector2i(0, 0)).x - CAMERAMOVESPEED * frameTime.asSeconds() > 1)
					camera.move(-CAMERAMOVESPEED * frameTime.asSeconds(), 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
				if (window.mapPixelToCoords(sf::Vector2i(camera.getSize() / zoomFactor)).y + CAMERAMOVESPEED * frameTime.asSeconds() < canvas.getTileGrid()->size() * 25.f - 1)
					camera.move(0, CAMERAMOVESPEED * frameTime.asSeconds());
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
				if (window.mapPixelToCoords(sf::Vector2i(camera.getSize() / zoomFactor)).x + CAMERAMOVESPEED * frameTime.asSeconds() < canvas.getTileGrid()->at(0).size() * 25.f - 1)
					camera.move(CAMERAMOVESPEED * frameTime.asSeconds(), 0);
			}
		}


		//Mouse Action Logic
		if (mouseAction == MouseAction::painting) {
			canvas.paintTile(window.mapPixelToCoords(sf::Mouse::getPosition(window)), selectedColor);
		}
		else if (mouseAction == MouseAction::erasing) {
			canvas.eraseTile(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		}
		else if (mouseAction == MouseAction::fogging) {
			canvas.fogTile(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		}
		else if (mouseAction == MouseAction::unfogging) {
			canvas.unfogTile(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		}
		else if (mouseAction == MouseAction::tokenMoving) {
			selectedToken->setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		}
		else if (mouseAction == MouseAction::tokenResizing) {
			sf::Vector2f currentMouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

			sf::Vector2i newSize = originalSize + sf::Vector2i((currentMouse - mouseOrigin) / TILESIZE);
			if (newSize.x > 0 && newSize.y > 0)
				selectedToken->setSize(newSize);


		}


		window.setView(camera);
		window.clear(sf::Color::White);


		//Drawing Tiles
		canvas.update();
		canvas.draw(window);

		//Update and Draw the UI
		ui.updateElementScales(zoomFactor);
		ui.updateElementPositions();
		ui.drawElements();


		//Colorwheel Drawing
		if(mouseAction == MouseAction::colorPicking)
			window.draw(colorWheel);


		//FPS Text Drawing
		window.draw(fpsText);


		//DISPLAY
		window.display();
	}
}

//Logic for all of the event handling. Called from main loop.
void Manager::interpretEvent(sf::Event pollingEvent){


	//Switch Statement Logic.
	switch (pollingEvent.type) {


	case sf::Event::Closed:
		window.close();
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
			sf::Vector2f beforeMouseLoc = window.mapPixelToCoords(sf::Mouse::getPosition(window));
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

			window.setView(camera);

			sf::Vector2f afterMouseLoc = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			sf::Vector2f moveVector = beforeMouseLoc - afterMouseLoc;
			camera.move(moveVector.x, moveVector.y);

			window.setView(camera);

			//Logic for keeping camera within bounds.
			if (window.mapPixelToCoords(sf::Vector2i(0, 0)).x < 1) {
				camera.move(1 - window.mapPixelToCoords(sf::Vector2i(0, 0)).x, 0);
			}
			if (window.mapPixelToCoords(sf::Vector2i(0, 0)).y < 1) {
				camera.move(0, 1 - window.mapPixelToCoords(sf::Vector2i(0, 0)).y);
			}
			if (window.mapPixelToCoords(sf::Vector2i(camera.getSize() / zoomFactor)).x > canvas.getTileGrid()->at(0).size() * 25.f - 1) {
				camera.move(canvas.getTileGrid()->at(0).size() * 25.f - 1 - window.mapPixelToCoords(sf::Vector2i(camera.getSize() / zoomFactor)).x, 0);
			}
			if (window.mapPixelToCoords(sf::Vector2i(camera.getSize() / zoomFactor)).y > canvas.getTileGrid()->size() * 25.f - 1) {
				camera.move(0, canvas.getTileGrid()->size() * 25.f - 1 - window.mapPixelToCoords(sf::Vector2i(camera.getSize() / zoomFactor)).y);
			}

			window.setView(camera);
		}
		break;


	case sf::Event::MouseButtonPressed:
	{ //Scope needed to prevent initialization error on mouseWindowLocation variable.
		sf::Vector2i mouseWindowLocation = sf::Mouse::getPosition(window);

		switch (pollingEvent.mouseButton.button) {
		case sf::Mouse::Button::Middle:
			isPanning = true;
			panLockLoc = window.mapPixelToCoords(mouseWindowLocation);
			window.setMouseCursor(handCursor);
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
							selectedToken = canvas.getClickedToken(window.mapPixelToCoords(mouseWindowLocation));
							if (selectedToken == nullptr) {
								canvas.createToken(window.mapPixelToCoords(mouseWindowLocation), selectedColor);
							}
							else {
								mouseAction = MouseAction::tokenResizing;
								mouseOrigin = window.mapPixelToCoords(mouseWindowLocation);
								originalSize = selectedToken->getSize();
							}

						}
						else {
							selectedToken = canvas.getClickedToken(window.mapPixelToCoords(mouseWindowLocation));
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
				screenshotTexture.create(window.getSize().x, window.getSize().y);
				screenshotTexture.update(window);
				sf::Image screenshot = screenshotTexture.copyToImage();
				sf::Vector2i mouseLoc = sf::Mouse::getPosition(window);
				selectedColor = screenshot.getPixel(mouseLoc.x, mouseLoc.y);

				mouseAction = MouseAction::none;
				break;
			}

			case MouseAction::changingName:
				selectedToken->setIsEditing(false);
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
						canvas.eraseToken(window.mapPixelToCoords(mouseWindowLocation));
					}
					else { //Non shift Right Click
						selectedToken = canvas.getClickedToken(window.mapPixelToCoords(mouseWindowLocation));
						if (selectedToken != nullptr) {
							selectedToken->setIsEditing(true);
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
			window.setMouseCursor(defaultCursor);
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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)) { //Shift is pressed, add upper case letter.
					selectedToken->addNameLetter(UPPERCASEALPHABET[newKey]);
				}
				else { //Shift isn't pressed, add lower case letter.
					selectedToken->addNameLetter(LOWERCASEALPHABET[newKey]);
				}
			}
			else if (pollingEvent.key.code == sf::Keyboard::Key::Backspace) {
				selectedToken->removeNameLetter();
			}
			else if (pollingEvent.key.code == sf::Keyboard::Key::Enter) {
				selectedToken->setIsEditing(false);
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
				colorWheel.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
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
