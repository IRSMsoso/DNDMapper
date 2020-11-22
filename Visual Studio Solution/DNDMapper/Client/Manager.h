#pragma once
#include "Game.h"
#include "MainMenu.h"
#include "NetworkManager.h"
#include <assert.h>





//Manager class, used to manage everything game related.
class Manager{
public:
	Manager(sf::ContextSettings);
	~Manager();

	//Main loop. Called as its own thread.
	void mainLoop();

private:
	//Window
	sf::RenderWindow window;

	//Network Manager
	NetworkManager networkManager;

	std::vector<std::unique_ptr<Menu>> menuStack;
};
