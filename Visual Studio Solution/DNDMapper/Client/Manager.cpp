#include "Manager.h"

//Construct the Manager with the specific window settings.
Manager::Manager(sf::ContextSettings settings): window(sf::VideoMode(WINDOWX, WINDOWY), "Dungeons and Dragons!", sf::Style::Close | sf::Style::Titlebar, settings), networkThread(&NetworkManager::listenForMessages, &networkManager){

	menuStack.push_back(std::unique_ptr<MainMenu>(new MainMenu(&window, &menuStack, &networkManager)));
	/*
	if (!networkManager.connect(sf::IpAddress("173.26.223.180"))) {
		window.close();
		return;
	}

	//Send version number.
	Command sendCommand;
	sendCommand.type = CommandType::VersionConfirmation;
	sendCommand.version = VERSION;
	networkManager.sendCommand(sendCommand);

	bool canProceed = false;

	while (!canProceed) {
		std::vector<Command> commands =  networkManager.getCommandsFromType(CommandType::VersionConfirmation);

		for (int i = 0; i < commands.size(); i++) {
			if (commands.at(i).version == VERSION) {
				std::cout << "Version Match Confirmed. Running...\n";
				canProceed = true;
			}
		}
		std::cout << "Waiting for Version Match Confirmation...\n";
		sf::sleep(sf::seconds(1));
	}
	networkThread.launch();
	std::cout << "Networking Thread Launched...\n";
	*/
}

Manager::~Manager(){

}

//The main event loop of the Manager object.
void Manager::mainLoop() {

	while (window.isOpen()) {
		sf::Event pollingEvent;
		while (window.pollEvent(pollingEvent)) {
			menuStack.back()->interpretEvent(pollingEvent);
		}

		menuStack.back()->update();

		//std::cout << "WINDOW: " << window.getSize().x << ", " << window.getSize().y << std::endl;

		window.clear();
		window.setView(menuStack.back()->getCamera());
		window.draw(*menuStack.back());
		window.display();
	}
}
