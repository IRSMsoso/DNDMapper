#include "Game.h"


Game::Game(sf::Uint16 newID, Player* newDM) {
	id = newID;
	DM = newDM;
	DM->setGame(this);
}

Game::~Game(){

}

bool Game::isDM(Player * compPlayer)
{
	return (compPlayer == DM);
}

bool Game::removePlayer(Player* player)
{
	for (int i = 0; i < players.size(); i++) {
		if (players.at(i) == player) {
			players.erase(players.begin() + i);
			return true;
		}
	}

	return false;
}

bool Game::addPlayer(Player* player) {
	bool proceed = true;
	for (int i = 0; i < players.size(); i++) {
		if (players.at(i) == player)
			proceed = false;
	}
	if (proceed) {
		players.push_back(player);
		return true;
	}
	return false;
}

//Passes the message to every player in the game except the player passed.
void Game::passMessage(sf::Packet packet, Player* player) {
	for (int i = 0; i < players.size(); i++) {
		if (players.at(i) != player) {
			players.at(i)->getSocket()->send(packet);
		}
	}
}
