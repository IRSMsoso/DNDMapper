#pragma once
#include "Game.h"
#include "MainMenu.h"
#include "NetworkManager.h"


const sf::Uint16 VERSION = 1;


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
	sf::Thread networkThread;

	std::vector<std::unique_ptr<Menu>> menuStack;
};
