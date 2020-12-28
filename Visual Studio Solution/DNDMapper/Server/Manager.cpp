#include "Manager.h"

Manager::Manager(){
	isRunning = false;
}

Manager::~Manager() {

}

void Manager::run(){
	std::cout << "Started Running Manager.\n";
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

					std::cout << "Message:\nType: " << inCommand.type << "\nName: " << inCommand.name << "\nID: " << inCommand.id << "\nVersion: " << inCommand.version << std::endl;

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
					else if (inCommand.type == CommandType::CreateGame) {
						Command outCommand;
						outCommand.type = CommandType::GameID;
						outCommand.id = createGame(player);
						sf::Packet outPacket;
						outPacket << outCommand;
						player->getSocket()->send(outPacket);
						std::cout << "Created Game and Sent Game ID\n";
						
					}
					else if (inCommand.type == CommandType::ConnectGame) { //If command is join game command.
						bool shouldSendFail = true;
						for (int i = 0; i < runningGames.size(); i++) { //For each game.
							if (runningGames.at(i)->getID() == inCommand.id) { //If ID of game is the one desired.
								if (runningGames.at(i)->addPlayer(player)) { //If successfully added.
									std::cout << "Successfully added player to game.\n";
								}
								else {
									std::cout << "Error adding player to game. Game already contains player\n";
								}
								//Regardless if they're already apart, we want to send back that it's okay to go to the game screen on client.
								Command outCommand;
								outCommand.type = CommandType::ConnectGame;
								outCommand.OK = true;
								std::cout << "outCommand True\n";
								outCommand.id = inCommand.id;
								sf::Packet outPacket;
								outPacket << outCommand;
								player->getSocket()->send(outPacket);
								shouldSendFail = false;
							}
						}
						if (shouldSendFail) {
							Command outCommand;
							outCommand.type = CommandType::ConnectGame;
							outCommand.OK = false;
							sf::Packet outPacket;
							outPacket << outCommand;
							player->getSocket()->send(outPacket);
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

sf::Uint16 Manager::createGame(Player* newDM) {
	sf::Uint16 id = 1;
	for (int i = 0; i < runningGames.size(); i++) {
		if (runningGames.at(i)->getID() >= id)
			id = runningGames.at(i)->getID() + 1;
	}
	
	Game* game = new Game(id, newDM);
	runningGames.push_back(game);

	std::cout << "Created Game.\n";

	return id;
}
