#include "Player.h"

Player::Player(){
	socket = nullptr;
	currentGame = nullptr;
}

Player::~Player() {
	delete socket;
}

bool Player::sendOnMessage(Command command)
{
	return false;
}
