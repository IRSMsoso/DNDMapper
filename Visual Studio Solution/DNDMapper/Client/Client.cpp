// Client.cpp : Client Main File
//

#include <iostream>
#include "Canvas.h"

int main()
{
	//Setup
	sf::RenderWindow window(sf::VideoMode(800, 800), "Dungeons and Dragons!");
	sf::View camera(sf::FloatRect(0.f, 0.f, 800, 800));
	window.setView(camera);

	sf::CircleShape circle(50.f, 100);
	circle.setOrigin(50, 50);
	circle.setFillColor(sf::Color::Black);


	//Window Loop
	while (window.isOpen()) {
		sf::Event pollingEvent;
		while (window.pollEvent(pollingEvent)) {
			if (pollingEvent.type == sf::Event::Closed)
				window.close();
			if (pollingEvent.type == sf::Event::MouseWheelScrolled) {
				if (pollingEvent.mouseWheelScroll.delta > 0) {
					camera.zoom(0.6);
					std::cout << "Success" << std::endl;
				}
				if (pollingEvent.mouseWheelScroll.delta < 0) {
					camera.zoom(1.4);
				}
				window.setView(camera);
			}
		}
		circle.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		window.clear(sf::Color::White);
		window.draw(circle);
		window.display();
	}

	return 0;
}