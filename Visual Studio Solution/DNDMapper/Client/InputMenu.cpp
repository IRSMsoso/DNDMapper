#include "InputMenu.h"

InputMenu::InputMenu(sf::RenderWindow * newWindow, std::vector<std::unique_ptr<Menu>>* newStack, NetworkManager * newNetworkManager, std::unique_ptr<Menu> newNextMenu) : Menu(newWindow, newStack, newNetworkManager) {
	nextMenu = std::move(newNextMenu);
	text.setPosition(500, 500);
	text.setFillColor(sf::Color::White);
	text.setScale(1, 1);
	algerFont.loadFromFile("ALGER.TTF");
	text.setFont(algerFont);
	window->setKeyRepeatEnabled(true);
	connectingTo = 0;
}

InputMenu::~InputMenu() {
	window->setKeyRepeatEnabled(false); //Might be in the wrong place.
}

void InputMenu::interpretEvent(sf::Event pollingEvent){
	switch (pollingEvent.type) {
	case sf::Event::Closed:
		close();
		break;

	case sf::Event::KeyPressed:
		std::cout << "Key Pressed: " << pollingEvent.key.code << std::endl;

		int newKey = -1;
		std::cout << "Code: " << pollingEvent.key.code << std::endl;
		for (int i = 0; i < (sizeof(NUMBERSALLOWEDKEYS) / sizeof(*NUMBERSALLOWEDKEYS)); i++) {
			if (NUMBERSALLOWEDKEYS[i] == pollingEvent.key.code)
				newKey = i;
		}

		
		
		if (newKey != -1) {
			text.setString(text.getString() + NUMBERSALPHABET[newKey]);
			std::cout << "Set new string to " << (std::string)text.getString() << std::endl;
		}
		else if (pollingEvent.key.code == 59) {
			text.setString(text.getString().substring(0, text.getString().getSize() - 1));
		}
		else if (pollingEvent.key.code == 58) {
			//Enter pressed. Time to move on from this Menu.
			if (text.getString().getSize() == 5 && connectingTo == 0) {
				connectingTo = std::stoi(text.getString().toAnsiString());
				std::cout << "Connecting to: " << connectingTo << std::endl;
				Command connectCommand;
				connectCommand.type = CommandType::connectGame;
				connectCommand.id = connectingTo;
				networkManager->sendCommand(connectCommand);

			}
		}
		
		

		break;
	}
}

void InputMenu::update() {
	if (!networkManager->getIsConnected()) {
		close();
		return; //Don't need the rest of this function.
	}

	std::vector<Command> connectCommands = networkManager->getCommandsFromType(CommandType::connectGame);
	if (connectCommands.size() > 1)
		std::cout << "Error, got multiple connect commands when not expected.\n";

	if (connectCommands.size() == 1) {
		if (connectCommands.at(0).OK) {
			menuStack->push_back(std::unique_ptr<Game>(new Game(window, menuStack, networkManager, GameAction::newGame))); //NewGame for gameaction is placeholder.
			close();
		}
		else {
			connectingTo = 0;
		}
	}
}

void InputMenu::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(text);
}
