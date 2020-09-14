#include "NetworkManager.h"

NetworkManager::NetworkManager(){
	connected = false;
}

bool NetworkManager::connect(sf::IpAddress address) {
	if (socket.connect(address, 51248) == sf::TcpSocket::Status::Done) {
		std::cout << "Successfully connected to the network.\n";
		connected = true;
	}
	else {
		std::cout << "Connection to network failed.\n";
		connected = false;
	}
}

std::vector<Command> NetworkManager::getCommandsFromType(CommandType type) {
	std::vector<Command> returnCommands;
	for (int i = 0; i < commandQueue.size(); i++) {
		if (commandQueue.at(i).type == type) {
			returnCommands.push_back(commandQueue.at(i));
			commandQueue.erase(commandQueue.begin() + i);
			i--;
		}
	}

	return returnCommands;
}

