#include "Menu.h"

Menu::Menu(sf::RenderWindow* newWindow, std::vector<std::unique_ptr<Menu>>* newStack): camera(sf::FloatRect(0.f, 0.f, WINDOWX, WINDOWY)) {
	shouldClose = false;

	window = newWindow;

	menuStack = newStack;

}

Menu::~Menu(){



}
