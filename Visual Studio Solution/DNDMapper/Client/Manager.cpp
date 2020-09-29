#include "Manager.h"

//Construct the Manager with the specific window settings.
Manager::Manager(sf::ContextSettings settings): window(sf::VideoMode(WINDOWX, WINDOWY), "Dungeons and Dragons!", sf::Style::Close | sf::Style::Titlebar, settings), networkThread(&NetworkManager::listenForMessages, &networkManager){

	menuStack.push_back(std::unique_ptr<MainMenu>(new MainMenu(&window, &menuStack, &networkManager)));
	/*
	if (!networkManager.connect(sf::IpAddress("173.26.223.180"))) {
		window.close();
		return;
	}

	networkThread.launch();
	std::cout << "Networking Thread Launched...\n";

	//Send version number.
	Command sendCommand;
	sendCommand.type = CommandType::VersionConfirmation;
	sendCommand.version = VERSION;
	sf::Socket::Status status = networkManager.sendCommand(sendCommand);

	bool canProceed = false;

	while (!canProceed) {
		std::vector<Command> commands =  networkManager.getCommandsFromType(CommandType::VersionConfirmation);

		for (int i = 0; i < commands.size(); i++) {
			if (commands.at(i).version == VERSION) {
				std::cout << "Version Match Confirmed. Running...\n";
				canProceed = true;
			}
			else {
				std::cout << "Version Mismatch.. Closing.\n";
				networkManager.shutdown();
				window.close();
				return;
			}
			
		}
		status = networkManager.sendCommand(sendCommand); //Send Again
		std::cout << "Waiting for Version Match Confirmation... " << status << "\n";
		sf::sleep(sf::seconds(1));
	}
	*/
	
}

Manager::~Manager(){

}

//The main event loop of the Manager object.
void Manager::mainLoop() {

	while (window.isOpen() && networkManager.isOperational() && menuStack.size() > 0) {

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
