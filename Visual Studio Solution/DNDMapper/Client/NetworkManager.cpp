#include "NetworkManager.h"

NetworkManager::NetworkManager(): connectThread(&NetworkManager::connect, this), listenThread(&NetworkManager::listenForMessages, this){
	ipAddress = sf::IpAddress::None;
	connected = false;
	versionReady = false;
	SERVER_VERSION = 0;
}

bool NetworkManager::startConnect(sf::IpAddress address) {

	if (!connected) {
		std::cout << "Starting the thread to connect to the server...\n";

		ipAddress = address;
		connectThread.launch();
		return true;
	}
	else {
		std::cout << "Error, tried to start connect thread but client is already connected.\n";
		return false;
	}
}

void NetworkManager::connect() {

	if (ipAddress != sf::IpAddress::None && socket.connect(ipAddress, 51248) == sf::TcpSocket::Status::Done) {
		std::cout << "Successfully connected to the network. Launching Listening Thread.\n";
		listenThread.terminate(); //Incase the thread hasn't stopped from a previous connection period.
		listenThread.launch();
		connected = true;

		//Make sure version is correct.
		Command versionCommand;
		versionCommand.type = CommandType::VersionConfirmation;
		versionCommand.version = VERSION;
		sendCommand(versionCommand);

	}
	else {
		std::cout << "Connection to network failed. Clearing Stored IP Address\n";
		ipAddress = sf::IpAddress::None;
		connected = false;
	}
}

//Get all commands that are of a certain message type and delete them from the backlog.
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


//Get and delete all messages that are related to updating the canvas.
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
	commandQueueMutex.unlock();


	return returnCommands;
}

//Send a message through the Network Manager.
sf::Socket::Status NetworkManager::sendCommand(Command command) {
	
	sf::Packet outPacket;
	outPacket << command;
	sf::Socket::Status status = socket.send(outPacket);

	std::cout << "Successfully Sent Command.\n";
	return status;
}

void NetworkManager::listenForMessages() {

	while (connected) {

		sf::Packet incomingPacket;

		sf::Socket::Status status = socket.receive(incomingPacket);
		if (status == sf::Socket::Status::Done) {
			std::cout << "Received Message\n";
			Command newCommand;
			incomingPacket >> newCommand;

			//Handles version checks itself.
			if (newCommand.type = CommandType::VersionConfirmation) {
				SERVER_VERSION = newCommand.version;
			}
			else { //Rest of the messages.
				commandQueueMutex.lock();

				commandQueue.push_back(newCommand);

				commandQueueMutex.unlock();
			}

		}
		else if (status == sf::Socket::Disconnected) {
			std::cout << "Disconnected from the server... Shutting down network manager.\n";
			resetManager();
		}
		else {
			std::cout << "Unknown networking error occured.\n";
		}
	}

	std::cout << "Shutting down Network Manager.\n";
}

//Shutdown the Network Manager and reset it to default settings.
void NetworkManager::shutdown() {
	std::cout << "Shutting Down Network Manager.\n";
	socket.disconnect();
	resetManager();

}

//Reset the network manager to default settings and terminates all threads. startConnect() will need to be called again.
void NetworkManager::resetManager() {
	std::cout << "Resetting Network Manager.\n;";
	connected = false;
	versionReady = false;
	SERVER_VERSION = 0;
	ipAddress = sf::IpAddress::None;
	listenThread.terminate();
	connectThread.terminate();
}

