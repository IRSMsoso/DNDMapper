#include "InputMenu.h"

InputMenu::InputMenu(sf::RenderWindow * newWindow, std::vector<std::unique_ptr<Menu>>* newStack, NetworkManager * newNetworkManager, std::unique_ptr<Menu> newNextMenu) : Menu(newWindow, newStack, newNetworkManager) {
	nextMenu = std::move(newNextMenu);
	text.setPosition(500, 500);
	text.setFillColor(sf::Color::White);
	text.setScale(1, 1);
	algerFont.loadFromFile("ALGER.TTF");
	text.setFont(algerFont);
}

void InputMenu::interpretEvent(sf::Event pollingEvent){
	switch (pollingEvent.type) {
	case sf::Event::Closed:
		close();
		break;

	case sf::Event::KeyPressed:
		std::cout << "Key Pressed: " << pollingEvent.key.code << std::endl;

		int newKey = -1;
		std::cout << "Size: " << (sizeof(ALLOWEDKEYS) / sizeof(*ALLOWEDKEYS)) << std::endl;
		for (int i = 0; i < (sizeof(ALLOWEDKEYS) / sizeof(*ALLOWEDKEYS)); i++) {
			if (ALLOWEDKEYS[i] == pollingEvent.key.code)
				newKey = i;
		}

		
		
		if (newKey != -1) {
			if (pollingEvent.key.shift) { //Shift is pressed, add upper case letter.
				text.setString(text.getString() + UPPERCASEALPHABET[newKey]);
			}
			else { //Shift isn't pressed, add lower case letter.
				text.setString(text.getString() + LOWERCASEALPHABET[newKey]);
			}
			std::cout << "Set new string to " << (std::string)text.getString() << std::endl;
		}
		

		break;
	}
}

void InputMenu::update() {

}

void InputMenu::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(text);
}
