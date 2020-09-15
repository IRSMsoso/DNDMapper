#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "NetworkStructs.h"





//Network Manager Class.
class NetworkManager {

public:
	NetworkManager();

	bool connect(sf::IpAddress);

	std::vector<Command> getCommandsFromType(CommandType); //Returns a vector of all the commands with that matching id, and removes them from the queue.
	std::vector<Command> getCanvasUpdateCommands();

	sf::Socket::Status sendCommand(Command); //Send a command to the server.

	void listenForMessages(); //Threaded function that just sits and listens for messages, putting them in the queue when it receives them.

	void shutdown() { isOn = false; } //Makes isOn false, *ideally* stopping the listenForMessages loop.
	bool isOperational(){return isOn;}

private:
	sf::TcpSocket socket;

	sf::Mutex commandQueueMutex;

	std::vector<Command> commandQueue;
	
	bool connected;

	bool isOn;
};

