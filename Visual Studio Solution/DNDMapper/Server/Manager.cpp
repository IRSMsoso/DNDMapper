#include "Manager.h"

Manager::Manager(){
	isRunning = false;
}

Manager::~Manager() {

}

void Manager::run(){
	isRunning = true;
	listener.listen(51248);
	socketSelector.add(listener);

	while (isRunning) {

		socketSelector.wait();

		std::cout << "Selector Received Something\n";

		if (socketSelector.isReady(listener)) {
			sf::TcpSocket* newSocket = new sf::TcpSocket;
			listener.accept(*newSocket);
			Player* newPlayer = new Player();
			newPlayer->setSocket(newSocket);
			connectedPlayers.push_back(newPlayer);
			socketSelector.add(*newSocket);
			std::cout << "New Player Added.\n";
		}

		for (int i = 0; i < connectedPlayers.size(); i++) {
			if (socketSelector.isReady(*connectedPlayers.at(i)->getSocket())) {
				sf::Packet inPacket;
				sf::Socket::Status socketStatus = connectedPlayers.at(i)->getSocket()->receive(inPacket);
				if (socketStatus == sf::Socket::Status::Done) {
					std::cout << "Received Message\n";
					Command inCommand;
					inPacket >> inCommand;
					if (inCommand.type == CommandType::VersionConfirmation) {
						Command outCommand;
						outCommand.type = CommandType::VersionConfirmation;
						outCommand.version = VERSION;
						sf::Packet outPacket;
						outPacket << outCommand;
						connectedPlayers.at(i)->getSocket()->send(outPacket);
						if (outCommand.version != VERSION) {
							(removePlayer(connectedPlayers.at(i)));
							i--;
						}
					}
					//If not version, pass on to other players in game.
					else {

						

					}


				}
				else if (socketStatus == sf::Socket::Disconnected) {
					std::cout << "Player disconnected.\n";
					removePlayer(connectedPlayers.at(i));
				}
				else {
					std::cout << "Unknown networking error occured.\n";
				}
			}
		}

	}
}

bool Manager::removePlayer(Player* player) {
	for (int i = 0; i < connectedPlayers.size(); i++) {
		if (connectedPlayers.at(i) == player) {
			//Remove them from the game they're in.
			if (player->getGame() != nullptr) {
				player->getGame()->removePlayer(player);
			}
			//Rest of it.
			socketSelector.remove(*player->getSocket());
			connectedPlayers.erase(connectedPlayers.begin() + i);
			std::cout << "Removed Player\n";
			delete player;
			return true;
		}
	}
	return false;
}
