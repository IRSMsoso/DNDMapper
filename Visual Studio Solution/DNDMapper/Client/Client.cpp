// Client.cpp : Client Main File
//

#include <iostream>
#include "Manager.h"



int main()
{
	//Setup
	//Window
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	//Creating manager, passing in window settings.
	Manager manager(settings);

	Tab tab;

	manager.mainLoop();

	return 0;
}