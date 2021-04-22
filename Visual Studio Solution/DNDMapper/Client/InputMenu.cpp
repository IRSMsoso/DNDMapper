#include "InputMenu.h"

InputMenu::InputMenu(MenuInfo menuInfo, GameAction purpose) : Menu(menuInfo) {
	text.setPosition(500, 500);
	text.setFillColor(sf::Color::White);
	text.setScale(1, 1);
	text.setFont(*resourceManager->getFontResource("arialfont"));
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
		//std::cout << "Key Pressed: " << pollingEvent.key.code << std::endl;

		int newKey = -1;
		//std::cout << "Code: " << pollingEvent.key.code << std::endl;
		if (m_purpose == GameAction::newGame || m_purpose == GameAction::loadGame) {
			for (int i = 0; i < (sizeof(ALLOWEDKEYS) / sizeof(*ALLOWEDKEYS)); i++) {
				if (ALLOWEDKEYS[i] == pollingEvent.key.code)
					newKey = i;
			}
		}
		else {
			for (int i = 0; i < (sizeof(NUMBERSALLOWEDKEYS) / sizeof(*NUMBERSALLOWEDKEYS)); i++) {
				if (NUMBERSALLOWEDKEYS[i] == pollingEvent.key.code)
					newKey = i;
			}
		}
		
		
		if (newKey != -1 and text.getString().getSize() <= 20) {
			if (m_purpose == GameAction::newGame || m_purpose == GameAction::loadGame)
				text.setString(text.getString() + LOWERCASEALPHABET[newKey]);
			else
				text.setString(text.getString() + NUMBERSALPHABET[newKey]);

			//std::cout << "Set new string to " << (std::string)text.getString() << std::endl;
		}
		else if (pollingEvent.key.code == 59) {
			text.setString(text.getString().substring(0, text.getString().getSize() - 1));
		}
		else if (pollingEvent.key.code == 58) {
			if (text.getString().getSize() > 0) {
				//Enter pressed. Time to move on from this Menu.
				if (m_purpose == GameAction::newGame || m_purpose == GameAction::loadGame) {
					DNDProto::NetworkMessage message;
					message.set_messagetype(DNDProto::NetworkMessage::MessageType::NetworkMessage_MessageType_CreateGame);
					networkManager->sendMessage(message);
					menuStack->push_back(std::unique_ptr<Game>(new Game(getMenuInfo(), m_purpose, text.getString())));
				}
				else if (m_purpose == GameAction::joinGame) {
					DNDProto::NetworkMessage message;
					message.set_messagetype(DNDProto::NetworkMessage::MessageType::NetworkMessage_MessageType_JoinGame);
					message.set_gameid(std::stoi(text.getString().toAnsiString()));
					networkManager->sendMessage(message);
					menuStack->push_back(std::unique_ptr<Game>(new Game(getMenuInfo(), m_purpose, text.getString())));
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
