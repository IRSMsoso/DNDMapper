#include "Game.h"

Game::Game(){

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
