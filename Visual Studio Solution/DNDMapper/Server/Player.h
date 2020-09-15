#pragma once
#include <SFML/Network.hpp>
#include "NetworkStructsServer.h"



class Game; //Forward declaration so that player can have a pointer to the game that it's in.

class Player {
public:

	Player();
	~Player();


	sf::TcpSocket* getSocket() { return socket; }
	void setSocket(sf::TcpSocket* newSocket) { socket = newSocket; }

	bool isInGame() { return !(currentGame == nullptr); }
	Game* getGame() { return currentGame; }

	bool sendOnMessage(Command);

private:

	Game* currentGame;

	sf::TcpSocket* socket;

};
