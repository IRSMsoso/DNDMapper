#pragma once
#include <iostream>
#include <vector>
#include <SFML/Network.hpp>

class Player{
public:

	Player();


	sf::TcpSocket* getSocket() { return socket; }


private:
	bool isDM;

	sf::TcpSocket* socket;

};

