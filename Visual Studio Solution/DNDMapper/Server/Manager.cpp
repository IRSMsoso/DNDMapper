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
			Player* player = connectedPlayers.at(i); //Pointer to current player in list.
			if (socketSelector.isReady(*player->getSocket())) {
				sf::Packet inPacket;
				sf::Socket::Status socketStatus = player->getSocket()->receive(inPacket);
				if (socketStatus == sf::Socket::Status::Done) {
					Command inCommand;
					inPacket >> inCommand;

					std::cout << "Message:\nType: " << inCommand.type << "\nName: " << inCommand.name << "\nID: " << inCommand.id << "\nVersion: " << inCommand.version;

					//If the command is a version confirmation.
					if (inCommand.type == CommandType::VersionConfirmation) {
						Command outCommand;
						outCommand.type = CommandType::VersionConfirmation;
						outCommand.version = VERSION;
						sf::Packet outPacket;
						outPacket << outCommand;
						player->getSocket()->send(outPacket);
						if (outCommand.version != VERSION) {
							(removePlayer(player));
							i--;
						}
					}
					else if (inCommand.type = CommandType::CreateGame) {
						createGame(inCommand.id, player);
					}
					else if (inCommand.type = CommandType::JoinGame) { //If command is join game command.
						for (int i = 0; i < runningGames.size(); i++) { //For each game.
							if (runningGames.at(i)->getID() == inCommand.id) { //If ID of game is the one desired.
								if (runningGames.at(i)->addPlayer(player)) { //If successfully added.
									std::cout << "Successfully added player to game.\n";
								}
								else {
									std::cout << "Error adding player to game. Game already contains player\n";
								}
							}
						}
					}
					//If not version, pass on to other players in game.
					else {

						Game* game = player->getGame(); //Get the game that the player is apart of.
						if (game != nullptr) {
							game->passMessage(inPacket, player); //Pass it on.
						}
					}


				}
				else if (socketStatus == sf::Socket::Disconnected) {
					std::cout << "Player disconnected.\n";
					removePlayer(connectedPlayers.at(i)); //Remove the player if they have disconnected.
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

void Manager::createGame(sf::Uint16 id, Player* newDM) {
	Game* game = new Game(id, newDM);
	runningGames.push_back(game);
}
