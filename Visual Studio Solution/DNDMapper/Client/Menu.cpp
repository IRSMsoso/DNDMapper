#include "Menu.h"

Menu::Menu(sf::RenderWindow* newWindow, std::vector<std::unique_ptr<Menu>>* newStack, NetworkManager* newNetworkManager): camera(sf::FloatRect(0.f, 0.f, WINDOWX, WINDOWY)) {
	shouldClose = false;

	window = newWindow;
	networkManager = newNetworkManager;

	menuStack = newStack;

}

Menu::~Menu(){



}
