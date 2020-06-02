// Client.cpp : Client Main File
//

#include <iostream>
#include "Canvas.h"

const unsigned int WINDOWX = 800;
const unsigned int WINDOWY = 800;


float lerp(float value, float start, float end)
{
	return start + (end - start) * value;
}


int main()
{
	//Setup
	sf::RenderWindow window(sf::VideoMode(WINDOWX, WINDOWY), "Dungeons and Dragons!");
	sf::View camera(sf::FloatRect(0.f, 0.f, WINDOWX, WINDOWY));
	window.setView(camera);

	bool isPanning = false;
	sf::Vector2f panLockLoc(0, 0);

	Canvas canvas;

	sf::Cursor handCursor, defaultCursor;
	handCursor.loadFromSystem(sf::Cursor::Hand);
	defaultCursor.loadFromSystem(sf::Cursor::Arrow);

	window.setMouseCursor(defaultCursor);


	//Window Loop
	while (window.isOpen()) {
		sf::Event pollingEvent;
		while (window.pollEvent(pollingEvent)) {
			if (pollingEvent.type == sf::Event::Closed)
				window.close();

			if (pollingEvent.type == sf::Event::MouseWheelScrolled) {
				float factor = 1.f;
				sf::Vector2f beforeMouseLoc = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				if (pollingEvent.mouseWheelScroll.delta > 0) {
					factor = 0.8;
				}
				if (pollingEvent.mouseWheelScroll.delta < 0) {
					factor = 1.2;
				}
				camera.zoom(factor);
				window.setView(camera);
				sf::Vector2f afterMouseLoc = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				std::cout << "Before: " << beforeMouseLoc.x << ", " << beforeMouseLoc.y << ", After: " << afterMouseLoc.x << ", " << afterMouseLoc.y << std::endl;
				sf::Vector2f moveVector =  beforeMouseLoc - afterMouseLoc;
				camera.move(moveVector);
				std::cout << "Moving: " << moveVector.x << ", " << moveVector.y << std::endl;
			}
			
			if (pollingEvent.type == sf::Event::MouseButtonPressed) {
				if (pollingEvent.mouseButton.button == sf::Mouse::Button::Middle) {
					isPanning = true;
					panLockLoc = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					window.setMouseCursor(handCursor);
				}
			}

			if (pollingEvent.type == sf::Event::MouseButtonReleased) {
				if (pollingEvent.mouseButton.button == sf::Mouse::Button::Middle) {
					isPanning = false;
					window.setMouseCursor(defaultCursor);
				}
			}

		}


		if (isPanning) {
			sf::Vector2f currentMouseLoc = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			sf::Vector2f moveVector = panLockLoc - currentMouseLoc;
			camera.move(moveVector);

		}


		window.setView(camera);
		window.clear(sf::Color::White);
		

		//Drawing Tiles
		sf::RectangleShape tileBrush(sf::Vector2f(5.f, 5.f));
		for (int y = 0; y < canvas.getTileGrid()->size(); y++) {
			for (int x = 0; x < canvas.getTileGrid()->at(y).size(); x++) {
				tileBrush.setPosition(x * 5, y * 5);
				tileBrush.setFillColor(canvas.getTileGrid()->at(y).at(x).getColor());
				window.draw(tileBrush);
			}
		}


		window.display();
	}

	return 0;
}