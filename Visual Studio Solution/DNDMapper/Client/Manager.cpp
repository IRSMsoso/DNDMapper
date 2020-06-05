#include "Manager.h"

//Construct the Manager with the specific window settings.
Manager::Manager(sf::ContextSettings settings): window(sf::VideoMode(WINDOWX, WINDOWY), "Dungeons and Dragons!", sf::Style::Default, settings), camera(sf::FloatRect(0.f, 0.f, WINDOWX, WINDOWY)), ui(&window){


	window.setView(camera);
	

	//Setup Initial Variable Values
	zoomFactor = 1;

	//Load Cursors
	handCursor.loadFromSystem(sf::Cursor::Hand);
	defaultCursor.loadFromSystem(sf::Cursor::Arrow);
	//Set cursor to default to start
	window.setMouseCursor(defaultCursor);

	selectedTool = ToolType::paintingTool;
}

Manager::~Manager(){

}

//The main event loop of the Manager object.
void Manager::mainLoop(){

	while (window.isOpen()) {

		//Interpret each event in queue from main thread.
		sf::Event pollingEvent;
		while (window.pollEvent(pollingEvent)) {
			interpretEvent(pollingEvent);
		}


		if (isPanning) {
			sf::Vector2f currentMouseLoc = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			sf::Vector2f moveVector = panLockLoc - currentMouseLoc;
			camera.move(moveVector.x, moveVector.y);

		}


		window.setView(camera);
		window.clear(sf::Color::White);


		//Drawing Tiles
		sf::RectangleShape tileBrush(sf::Vector2f(TILESIZE, TILESIZE));
		for (int y = 0; y < canvas.getTileGrid()->size(); y++) {
			for (int x = 0; x < canvas.getTileGrid()->at(y).size(); x++) {
				tileBrush.setPosition(x * TILESIZE, y * TILESIZE);
				tileBrush.setFillColor(canvas.getTileGrid()->at(y).at(x).getColor());
				window.draw(tileBrush);
			}
		}

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
				cornerBeads.setFillColor(sf::Color(rgbcircle, rgbcircle, rgbcircle, 255));

				window.draw(cornerBeads);
			}
		}


		//Drawing the Camera, along with all of the UI nested under it.
		//window.draw(camera);

		//Update and Draw the UI
		ui.updateElementScales(zoomFactor);
		ui.updateElementPositions();
		ui.drawElements();


		//std::cout << "Factor: " << zoomFactor << std::endl;

		window.display();
	}
}

//Logic for all of the event handling. Called from main loop.
void Manager::interpretEvent(sf::Event pollingEvent){
	if (pollingEvent.type == sf::Event::Closed)
		window.close();

	if (pollingEvent.type == sf::Event::MouseWheelScrolled) {
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

	if (pollingEvent.type == sf::Event::MouseButtonPressed) {
		sf::Vector2i mouseWindowLocation = sf::Mouse::getPosition(window);

		if (pollingEvent.mouseButton.button == sf::Mouse::Button::Middle) {
			isPanning = true;
			panLockLoc = window.mapPixelToCoords(mouseWindowLocation);
			window.setMouseCursor(handCursor);
		}

		if (pollingEvent.mouseButton.button == sf::Mouse::Button::Left) {
			ToolType newTool = ui.getToolClicked(mouseWindowLocation);

			if (newTool != ToolType::none){
				selectedTool = newTool;
				std::cout << "Selected Tool: " << selectedTool << std::endl;
			}
			else {
				if (selectedTool == ToolType::paintingTool) {
					canvas.paintTile(window.mapPixelToCoords(sf::Mouse::getPosition(window)), sf::Color::White);
				}
			}
		}
	}

	if (pollingEvent.type == sf::Event::MouseButtonReleased) {
		if (pollingEvent.mouseButton.button == sf::Mouse::Button::Middle) {
			isPanning = false;
			window.setMouseCursor(defaultCursor);
		}
	}
}
