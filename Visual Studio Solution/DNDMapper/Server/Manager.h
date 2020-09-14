#pragma once
#include <iostream>
#include "Game.h"


const sf::Uint16 VERSION = 1;

class Manager{
public:

	Manager();
	~Manager();

	void run();

private:
	std::vector<Player*> connectedPlayers;
	std::vector<Game*> runningGames;
};

