#include "InputMenu.h"

InputMenu::InputMenu(sf::RenderWindow * newWindow, std::vector<std::unique_ptr<Menu>>* newStack, NetworkManager * newNetworkManager) : Menu(newWindow, newStack, newNetworkManager) {

}

void InputMenu::interpretEvent(sf::Event pollingEvent){
	switch (pollingEvent.type) {
	case sf::Event::Closed:
		close();
		break;

	case sf::Event::KeyPressed:

	}
}

void InputMenu::update()
{
}

void InputMenu::draw(sf::RenderTarget &, sf::RenderStates) const
{
}
