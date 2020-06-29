#pragma once
#include <iostream>
#include "Game.h"


sf::Uint8 VERSION = 0;

class Manager{
public:

	Manager();
	~Manager();

	void run();

private:
	std::vector<Player*> connectedPlayers;
	std::vector<Game*> runningGames;
};

