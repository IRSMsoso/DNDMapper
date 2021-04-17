#include "InputMenu.h"

InputMenu::InputMenu(sf::RenderWindow * newWindow, std::vector<std::unique_ptr<Menu>>* newStack, NetworkManager * newNetworkManager, GameAction purpose) : Menu(newWindow, newStack, newNetworkManager) {
	text.setPosition(500, 500);
	text.setFillColor(sf::Color::White);
	text.setScale(1, 1);
	algerFont.loadFromFile("ALGER.TTF");
	text.setFont(algerFont);
	window->setKeyRepeatEnabled(true);
	m_purpose = purpose;
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
		for (int i = 0; i < (sizeof(ALLOWEDKEYS) / sizeof(*ALLOWEDKEYS)); i++) {
			if (ALLOWEDKEYS[i] == pollingEvent.key.code)
				newKey = i;
		}

		
		
		if (newKey != -1 and text.getString().getSize() <= 20) {
			text.setString(text.getString() + LOWERCASEALPHABET[newKey]);
			std::cout << "Set new string to " << (std::string)text.getString() << std::endl;
		}
		else if (pollingEvent.key.code == 59) {
			text.setString(text.getString().substring(0, text.getString().getSize() - 1));
		}
		else if (pollingEvent.key.code == 58) {
			if (text.getString().getSize() > 0) {
				//Enter pressed. Time to move on from this Menu.
				if (m_purpose == GameAction::newGame || m_purpose == GameAction::loadGame) {
					menuStack->push_back(std::unique_ptr<Game>(new Game(window, menuStack, networkManager, m_purpose, text.getString())));
				}
				else if (m_purpose == GameAction::joinGame) {
					//Push back connecting menu.
				}
				close();
			}
		}
		
		

		break;
	}
}

void InputMenu::update() {
	
}

void InputMenu::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(text);
}
