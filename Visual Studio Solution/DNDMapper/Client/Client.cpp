// Client.cpp : Client Main File
//

#include <iostream>
#include "Manager.h"




float lerp(float value, float start, float end)
{
	return start + (end - start) * value;
}


int main()
{
	//Setup
	//Window
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(WINDOWX, WINDOWY), "Dungeons and Dragons!", sf::Style::Default, settings);
	window.setActive(false);

	//Creating manager, passing in window settings.
	Manager manager(&window);

	//Create manager run thread and run it.
	sf::Thread runThread(&Manager::mainLoop, &manager);
	runThread.launch();

	//Window Loop
	while (window.isOpen()) {
		sf::Event pollingEvent;
		while (window.pollEvent(pollingEvent)) {
			if (pollingEvent.type == sf::Event::Closed)
				window.close();
			else
				if (pollingEvent.type == sf::Event::MouseButtonPressed)
					std::cout << "Pressed Event sent from main thread\n";
				manager.addEvent(pollingEvent);
		}
		
	}

	runThread.wait();

	return 0;
}