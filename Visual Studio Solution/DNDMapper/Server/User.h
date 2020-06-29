#pragma once
#include <iostream>
#include <vector>
#include <SFML/Network.hpp>


class User{
public:
	User();
	~User();

	sf::TcpSocket* getSocket() { return socket; }


protected:
	sf::TcpSocket* socket;

};

