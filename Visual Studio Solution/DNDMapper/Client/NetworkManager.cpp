#include "NetworkManager.h"

NetworkManager::NetworkManager(){
	connected = false;
	isOn = true;

}

bool NetworkManager::connect(sf::IpAddress address) {
	if (socket.connect(address, 51248) == sf::TcpSocket::Status::Done) {
		std::cout << "Successfully connected to the network.\n";
		connected = true;
		return true;
	}
	else {
		std::cout << "Connection to network failed.\n";
		connected = false;
		return false;
	}
}

std::vector<Command> NetworkManager::getCommandsFromType(CommandType type) {
	std::vector<Command> returnCommands;

	commandQueueMutex.lock();
	for (int i = 0; i < commandQueue.size(); i++) {
		if (commandQueue.at(i).type == type) {
			returnCommands.push_back(commandQueue.at(i));
			commandQueue.erase(commandQueue.begin() + i);
			i--;
		}
	}

	commandQueueMutex.unlock();

	return returnCommands;
}

std::vector<Command> NetworkManager::getCanvasUpdateCommands() {
	std::vector<Command> returnCommands;

	commandQueueMutex.lock();
	for (int i = 0; i < commandQueue.size(); i++) {
		if (1 <= commandQueue.at(i).type <= 7) {
			returnCommands.push_back(commandQueue.at(i));
			commandQueue.erase(commandQueue.begin() + i);
			i--;
		}
	}



	return std::vector<Command>();
}

sf::Socket::Status NetworkManager::sendCommand(Command command) {
	
	sf::Packet outPacket;
	outPacket << command;
	sf::Socket::Status status = socket.send(outPacket);

	std::cout << "Successfully Sent Command.\n";
	return status;
}

void NetworkManager::listenForMessages() {

	while (isOn) {

		sf::Packet incomingPacket;

		sf::Socket::Status status = socket.receive(incomingPacket);
		if (status == sf::Socket::Status::Done) {
			std::cout << "Received Message\n";
			Command newCommand;
			incomingPacket >> newCommand;


			commandQueueMutex.lock();

			commandQueue.push_back(newCommand);

			commandQueueMutex.unlock();

		}
		else if (status == sf::Socket::Disconnected) {
			std::cout << "Disconnected from the server... Shutting down network manager.\n";
			shutdown();
		}
		else {
			std::cout << "Unknown networking error occured.\n";
		}
	}

	std::cout << "Shutting down Network Manager.\n";
}

