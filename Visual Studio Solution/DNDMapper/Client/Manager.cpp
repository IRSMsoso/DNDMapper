#include "Manager.h"

//Construct the Manager with the specific window settings.
Manager::Manager(sf::ContextSettings settings): window(sf::VideoMode(WINDOWX, WINDOWY), "Dungeons and Dragons!", sf::Style::Default, settings), camera(sf::FloatRect(0.f, 0.f, WINDOWX, WINDOWY)), ui(&window){


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

	mouseAction == MouseAction::none;
	previousAction == MouseAction::none;
  
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


	//LOOP
	while (window.isOpen()) {


		//FPS
		sf::Time frameTime = fpsClock.restart();
		float fps = 1.f / frameTime.asSeconds();
		fpsText.setString(std::to_string(fps));


		//Interpret each event in queue from main thread.
		sf::Event pollingEvent;
		while (window.pollEvent(pollingEvent)) {
			interpretEvent(pollingEvent);
		}


		//Panning Logic
		if (isPanning) {
			sf::Vector2f currentMouseLoc = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			sf::Vector2f moveVector = panLockLoc - currentMouseLoc;
			camera.move(moveVector.x, moveVector.y);

		}

		//Keeping mouse in color wheel.
		if (mouseAction == MouseAction::colorPicking) {
			sf::Vector2f wheel = colorWheel.getPosition();
			sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			sf::Vector2f vectorBetween = wheel - mouse;
			float distance = sqrt(pow(vectorBetween.x, 2) + pow(vectorBetween.y, 2));
			sf::Vector2f unitVector = vectorBetween / distance;
			if (distance > WHEELRADIUS) {
				sf::Mouse::setPosition(sf::Vector2i(sf::Vector2f(sf::Mouse::getPosition()) + unitVector * (distance - WHEELRADIUS)));
			}
		}


		//Tool Click Logic
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


		window.setView(camera);
		window.clear(sf::Color::White);


		//Drawing Tiles
		canvas.update();
		canvas.draw(window);

		/*

		sf::CircleShape cornerBeads;

		cornerBeads.setRadius(BEADRADIUS);
		cornerBeads.setOrigin(BEADRADIUS, BEADRADIUS);
		for (int y = 1; y < canvas.getTileGrid()->size(); y++) {
			for (int x = 1; x < canvas.getTileGrid()->at(y).size(); x++) {
				cornerBeads.setScale(zoomFactor, zoomFactor);
				cornerBeads.setPosition(x * TILESIZE, y * TILESIZE);

				sf::Color TL = canvas.getTileGrid()->at(y - 1).at(x - 1).getColor();
				sf::Color TR = canvas.getTileGrid()->at(y - 1).at(x).getColor();
				sf::Color BL = canvas.getTileGrid()->at(y).at(x - 1).getColor();
				sf::Color BR = canvas.getTileGrid()->at(y).at(x).getColor();

				float avgR = ((float)TL.r + (float)TR.r + (float)BL.r + (float)BR.r) / 4.f;
				float avgG = ((float)TL.g + (float)TR.g + (float)BL.g + (float)BR.g) / 4.f;
				float avgB = ((float)TL.b + (float)TR.b + (float)BL.b + (float)BR.b) / 4.f;

				double rgbcircle = abs(((0.299 * avgR + 0.587 * avgG + 0.114 * avgB) / 255) - 1) * 255;
				//cornerBeads.setFillColor(sf::Color(rgbcircle, rgbcircle, rgbcircle, 255));

				//window.draw(cornerBeads);
			}
		}

		*/


		//Drawing the Camera, along with all of the UI nested under it.
		//window.draw(camera);

		//Update and Draw the UI
		ui.updateElementScales(zoomFactor);
		ui.updateElementPositions();
		ui.drawElements();


		//Colorwheel Drawing
		if(mouseAction == MouseAction::colorPicking)
			window.draw(colorWheel);

		//std::cout << "Factor: " << zoomFactor << std::endl;

		//FPS Text Drawing
		window.draw(fpsText);

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
					}
				}
			}
			break;

			case MouseAction::colorPicking:
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
			break;

		case sf::Mouse::Button::Right:

			switch (mouseAction) {
			case MouseAction::none:
				switch (selectedTool) {
				case ToolType::paintingTool:
					mouseAction = MouseAction::erasing;
					break;

				case ToolType::fogTool:
					mouseAction = MouseAction::unfogging;
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
				break;

			case MouseAction::fogging:
				mouseAction = MouseAction::none;
				break;
			}
			break;

		case sf::Mouse::Button::Right:
			switch (mouseAction) {
			case MouseAction::erasing:
				mouseAction = MouseAction::none;
				break;

			case MouseAction::unfogging:
				mouseAction = MouseAction::none;
				break;
			}
		}
		break;


	case sf::Event::KeyPressed:

		switch (pollingEvent.key.code) {
		case sf::Keyboard::E:
			if (mouseAction == MouseAction::none)
				previousAction = mouseAction;
			mouseAction = MouseAction::colorPicking;
			colorWheel.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			break;
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
