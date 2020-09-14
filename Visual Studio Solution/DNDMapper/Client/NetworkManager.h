#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

enum CommandType {
	VersionConfirmation,
	TokenCreated,
	TokenDeleted,
	TokenMoved,
	TokenRenamed,
	TilePainted,
	FogPainted,
	FogRemoved,
};

struct Command {

	CommandType type;
	sf::Uint16 version;
	sf::Uint16 id;
	sf::Vector2i gridLocation;
	sf::Color color;

};


class NetworkManager {

public:
	NetworkManager();

	bool connect(sf::IpAddress);

	std::vector<Command> getCommandsFromType(CommandType);

private:
	sf::TcpSocket socket;

	std::vector<Command> commandQueue;
	
	bool connected;
};

