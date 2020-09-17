#include "Player.h"

Player::Player(){
	socket = nullptr;
	currentGame = nullptr;
}

Player::~Player() {
	delete socket;
}
