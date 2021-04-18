#include "NetworkManager.h"

NetworkManager::NetworkManager(): connectThread(&NetworkManager::connect, this), listenThread(&NetworkManager::listenForMessages, this){
	ipAddress = sf::IpAddress::None;
	isConnected = false;
}

bool NetworkManager::startConnect(sf::IpAddress address) {
	if (!isConnected && !isConnecting) {
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
	isConnecting = true;

	if (ipAddress != sf::IpAddress::None && socket.connect(ipAddress, 51248) == sf::TcpSocket::Status::Done) {
		std::cout << "Successfully connected to the network. Launching Listening Thread.\n";
		listenThread.terminate(); //Incase the thread hasn't stopped from a previous connection period.
		listenThread.launch();
		isConnected = true;


	}
	else {
		std::cout << "Connection to network failed. Clearing Stored IP Address\n";
		ipAddress = sf::IpAddress::None;
		isConnected = false;
	}
	isConnecting = false;
}

//Get all commands that are of a certain message type and delete them from the backlog.
std::vector<DNDProto::NetworkMessage> NetworkManager::getMessagesOfType(DNDProto::NetworkMessage::MessageType type) {
	std::vector<DNDProto::NetworkMessage> returnMessages;

	messageQueueMutex.lock();
	for (int i = 0; i < messageQueue.size(); i++) {
		if (messageQueue.at(i).messagetype() == type) {
			returnMessages.push_back(messageQueue.at(i));
			messageQueue.erase(messageQueue.begin() + i);
			i--;
		}
	}

	messageQueueMutex.unlock();

	return returnMessages;
}


//Send a message through the Network Manager.
sf::Socket::Status NetworkManager::sendMessage(DNDProto::NetworkMessage message) {
	UINT32* length = new UINT32;
	*length = message.ByteSizeLong();
	std::cout << "Length_Bytes: " << (char*)length << std::endl; //WTF
	std::cout << "Length: " << *length << std::endl;
	void* data = new void*;
	message.SerializeToArray(data, *length);
	socket.send((void*)length, 4);
	sf::Socket::Status status = socket.send(data, *length);
	std::cout << "IS " << *length << " == " << sizeof(data) << std::endl;

	delete length;

	std::cout << "Successfully Sent Command.\n";
	return status;
}

void NetworkManager::listenForMessages() {
	while (isConnected) {

		messageQueueMutex.lock();
		if (messageQueue.size() > 0) {
			std::cout << "Command Queue Size: " << messageQueue.size();
		}
		messageQueueMutex.unlock();

		void* size_data = new void*;
		size_t size_message_size;
		socket.receive(size_data, 4, size_message_size);
		UINT32* size = (UINT32*)size_data;



		void* data = new void*;
		size_t actual_received;
		sf::Socket::Status status = socket.receive(data, *size, actual_received);
		if (status == sf::Socket::Status::Done) {
			std::cout << "Received Message\n";

			DNDProto::NetworkMessage message;
			message.ParseFromArray(data, *size);
			
			messageQueueMutex.lock();

			messageQueue.push_back(message);

			messageQueueMutex.unlock();

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
	isConnected = false;
	isConnecting = false;
	ipAddress = sf::IpAddress::None;
	listenThread.terminate();
	connectThread.terminate();
}

