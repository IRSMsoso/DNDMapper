#pragma once
#include <iostream>
#include "Game.h"



const sf::Uint16 VERSION = 1;

class Manager{
public:

	Manager();
	~Manager();

	void run();

	bool removePlayer(Player*);

	sf::Uint16 createGame(Player*);

private:

	sf::TcpListener listener;
	sf::SocketSelector socketSelector;

	std::vector<Player*> connectedPlayers;
	std::vector<Game*> runningGames;

	bool isRunning;
};

