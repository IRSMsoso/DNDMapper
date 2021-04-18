#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "DNDMapper.pb.h"


typedef unsigned char BYTE;

//Network Manager Class.
class NetworkManager {

public:
	NetworkManager();

	bool startConnect(sf::IpAddress);

	std::vector<DNDProto::NetworkMessage> getMessagesOfType(DNDProto::NetworkMessage::MessageType); //Returns a vector of all the commands with that matching id, and removes them from the queue.

	sf::Socket::Status sendMessage(DNDProto::NetworkMessage); //Send a command to the server.

	void listenForMessages(); //Threaded function that just sits and listens for messages, putting them in the queue when it receives them.

	void shutdown(); //Force disconnects the network manager.
	bool getIsConnected() { return isConnected; }

private:
	sf::TcpSocket socket;

	sf::Mutex messageQueueMutex;

	std::vector<DNDProto::NetworkMessage> messageQueue;

	//Thread for connecting to the server.
	bool isConnecting;
	void connect();
	sf::Thread connectThread;
	sf::IpAddress ipAddress;

	//Thread for listening to messages.
	sf::Thread listenThread;


	//Cleanup
	void resetManager();

	
	bool isConnected;

};

