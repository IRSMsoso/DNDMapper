// Client.cpp : Client Main File
//

#include <iostream>
#include "Manager.h"
#include <Windows.h>



int main()
{
	//ShowWindow(GetConsoleWindow(), SW_HIDE);

	//Setup
	//Window
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	//Creating manager, passing in window settings.
	Manager manager(settings);


	manager.mainLoop();

	//std::system("pause");

	return 0;
}