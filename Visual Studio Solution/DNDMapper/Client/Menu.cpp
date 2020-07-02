#include "Menu.h"

Menu::Menu(sf::RenderWindow* newWindow): camera(sf::FloatRect(0.f, 0.f, WINDOWX, WINDOWY)) {
	shouldClose = stayOpen;

	window = newWindow;

}

Menu::~Menu(){



}
