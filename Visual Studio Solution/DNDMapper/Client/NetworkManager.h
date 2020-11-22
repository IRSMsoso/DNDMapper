#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "NetworkStructs.h"


const sf::Uint16 VERSION = 1;


//Network Manager Class.
class NetworkManager {

public:
	NetworkManager();

	bool startConnect(sf::IpAddress);

	std::vector<Command> getCommandsFromType(CommandType); //Returns a vector of all the commands with that matching id, and removes them from the queue.
	std::vector<Command> getCanvasUpdateCommands();

	sf::Socket::Status sendCommand(Command); //Send a command to the server.

	void listenForMessages(); //Threaded function that just sits and listens for messages, putting them in the queue when it receives them.

	void shutdown(); //Force disconnects the network manager.
	bool isConnected() { return connected; }
	bool isVersionReady() { return versionReady; }
	bool isVersionCorrect() { return (VERSION == SERVER_VERSION); }

private:
	sf::TcpSocket socket;

	sf::Mutex commandQueueMutex;

	std::vector<Command> commandQueue;

	//Thread for connecting to the server.
	void connect();
	sf::Thread connectThread;
	sf::IpAddress ipAddress;

	//Thread for listening to messages.
	sf::Thread listenThread;


	//Version Handling.
	sf::Uint16 SERVER_VERSION;
	bool versionReady;


	//Cleanup
	void resetManager();

	
	bool connected;

};

