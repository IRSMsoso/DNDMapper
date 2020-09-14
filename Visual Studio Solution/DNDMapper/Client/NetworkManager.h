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
	sf::Vector2f worldLocation;
	sf::Color color;
	std::string name;

};

//Packet Extending. Not sure why I have to do this for sfml stuff, but whatever.
sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2i&);
sf::Packet& operator >>(sf::Packet& packet, sf::Vector2i&);
sf::Packet& operator <<(sf::Packet& packet, const sf::Color&);
sf::Packet& operator >>(sf::Packet& packet, sf::Color&);
sf::Packet& operator <<(sf::Packet& packet, const Command&);
sf::Packet& operator >>(sf::Packet& packet, Command&);
sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2f&);
sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f&);


//Network Manager Class.
class NetworkManager {

public:
	NetworkManager();

	bool connect(sf::IpAddress);

	std::vector<Command> getCommandsFromType(CommandType); //Returns a vector of all the commands with that matching id, and removes them from the queue.
	std::vector<Command> getCanvasUpdateCommands();

	bool sendCommand(Command); //Send a command to the server.

	void listenForMessages(); //Threaded function that just sits and listens for messages, putting them in the queue when it receives them.

	void shutdown() { isOn = false; } //Makes isOn false, *ideally* stopping the listenForMessages loop.

private:
	sf::TcpSocket socket;

	sf::Mutex commandQueueMutex;

	std::vector<Command> commandQueue;
	
	bool connected;

	bool isOn;
};

