// Client.cpp : Client Main File
//

#include <iostream>
#include "Manager.h"




float lerp(float value, float start, float end)
{
	return start + (end - start) * value;
}


int main()
{
	//Setup
	//Window
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	//Creating manager, passing in window settings.
	Manager manager(settings);

	manager.mainLoop();

	return 0;
}