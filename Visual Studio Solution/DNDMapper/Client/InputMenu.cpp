#include "InputMenu.h"

InputMenu::InputMenu(sf::RenderWindow * newWindow, std::vector<std::unique_ptr<Menu>>* newStack, NetworkManager * newNetworkManager, std::unique_ptr<Menu> newNextMenu) : Menu(newWindow, newStack, newNetworkManager) {
	nextMenu = std::move(newNextMenu);
}

void InputMenu::interpretEvent(sf::Event pollingEvent){
	switch (pollingEvent.type) {
	case sf::Event::Closed:
		close();
		break;

	case sf::Event::KeyPressed:
		std::cout << "Key Pressed: " << pollingEvent.key.code << std::endl;
		break;
	}
}

void InputMenu::update()
{
}

void InputMenu::draw(sf::RenderTarget &, sf::RenderStates) const
{
}
