#include "Manager.h"

//Construct the Manager with the specific window settings.
Manager::Manager(sf::ContextSettings settings): window(sf::VideoMode(WINDOWX, WINDOWY), "Dungeons and Dragons!", sf::Style::Close | sf::Style::Titlebar, settings) {

	menuStack.push_back(std::unique_ptr<MainMenu>(new MainMenu(&window, &menuStack, &networkManager)));

	//Start the connection to the server.
	networkManager.startConnect(sf::IpAddress("173.26.223.180"));

	//Send version number.
	Command sendCommand;
	sendCommand.type = CommandType::VersionConfirmation;
	sendCommand.version = VERSION;
	sf::Socket::Status status = networkManager.sendCommand(sendCommand);

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

		if (menuStack.back()->getShouldClose()) {
			menuStack.pop_back();
		}
	}
	window.close();
}
