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

bool NetworkManager::sendCommand(Command command) {
	
	sf::Packet outPacket;
	bool successPacket = outPacket << command;
	bool successSending = socket.send(outPacket);

	std::cout << "Successfully Sent Command.\n";
	return successPacket && successSending; //Returns true(succesful) only if both the packet and socket worked correctly.
}

void NetworkManager::listenForMessages() {

	while (isOn) {

		sf::Packet incomingPacket;

		socket.receive(incomingPacket);
		Command newCommand;
		incomingPacket >> newCommand;


		commandQueueMutex.lock();

		commandQueue.push_back(newCommand);

		commandQueueMutex.unlock();


	}

	std::cout << "Shutting down Network Manager.\n";
}


sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2i& vector) {
	sf::Int16 x = vector.x;
	sf::Int16 y = vector.y;
	return packet << x << y;
}

sf::Packet& operator >>(sf::Packet& packet, sf::Vector2i& vector) {
	return packet >> vector.x >> vector.y;
}

sf::Packet& operator <<(sf::Packet& packet, const sf::Color& color) {
	return packet << color.toInteger();
}

sf::Packet& operator >>(sf::Packet& packet, sf::Color& color) {
	sf::Uint32 colorInt;
	packet >> colorInt;
	color = sf::Color(colorInt);
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const Command& command) {
	return packet << command.type << command.version << command.id << command.gridLocation << command.color << command.name;
}

sf::Packet& operator >>(sf::Packet& packet, Command& command) {
	int typeint;
	packet >> typeint >> command.version >> command.id >> command.gridLocation >> command.color >> command.name;
	command.type = (CommandType)typeint;
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2f& worldxy) {
	return packet << worldxy.x << worldxy.y;
}

sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f& worldxy) {
	return packet >> worldxy.x >> worldxy.y;
}