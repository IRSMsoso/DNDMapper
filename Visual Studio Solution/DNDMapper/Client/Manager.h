#pragma once
#include "Game.h"
#include "MainMenu.h"





class Manager{
public:
	Manager(sf::ContextSettings);
	~Manager();

	//Main loop. Called as its own thread.
	void mainLoop();

private:
	//Window
	sf::RenderWindow window;

	std::vector<std::unique_ptr<Menu>> menuStack;
};
