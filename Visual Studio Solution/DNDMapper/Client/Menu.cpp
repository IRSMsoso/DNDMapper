#include "Menu.h"

Menu::Menu(MenuInfo menuInfo): camera(sf::FloatRect(0.f, 0.f, WINDOWX, WINDOWY)) {
	shouldClose = false;

	window = menuInfo.window;
	networkManager = menuInfo.networkManager;
	menuStack = menuInfo.menuStack;
	resourceManager = menuInfo.resourceManager;

}

Menu::~Menu(){



}

Menu::MenuInfo Menu::getMenuInfo()
{
	MenuInfo menuInfo;
	menuInfo.menuStack = menuStack;
	menuInfo.networkManager = networkManager;
	menuInfo.resourceManager = resourceManager;
	menuInfo.window = window;
	return menuInfo;
}
