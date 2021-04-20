#include "Manager.h"

//Construct the Manager with the specific window settings.
Manager::Manager(sf::ContextSettings settings): window(sf::VideoMode(WINDOWX, WINDOWY), "Dungeons and Dragons!", sf::Style::Close | sf::Style::Titlebar, settings) {

	menuStack.push_back(std::unique_ptr<MainMenu>(new MainMenu(&window, &menuStack, &networkManager)));

	//Start the connection to the server. Network Manager handles versioning.
	networkManager.startConnect(sf::IpAddress("173.24.79.165"));

	
	window.setFramerateLimit(144);
}

Manager::~Manager(){

}

//The main event loop of the Manager object.
void Manager::mainLoop() {

	while (window.isOpen()) {

		//Check for error involving menuStack.
		assert(menuStack.size() > 0);


		sf::Event pollingEvent;

		//Send Event to the last stack.
		while (window.pollEvent(pollingEvent)) {
			menuStack.back()->interpretEvent(pollingEvent);
		}

		menuStack.back()->update();

		//std::cout << "WINDOW: " << window.getSize().x << ", " << window.getSize().y << std::endl;

		window.clear();
		window.setView(menuStack.back()->getCamera());
		window.draw(*menuStack.back());
		window.display();

		
		//Free Memory for all Menus that need to be destroyed.
		for (int i = 0; i < menuStack.size(); i++) {
			if (menuStack.at(i)->getShouldClose()) {
				menuStack.erase(menuStack.begin() + i);
			}
		}
	}
	window.close();

	//Force Network Manager to shut down if the window closes.
	networkManager.shutdown();
}
