#include "Token.h"



Token::Token(sf::Color newColor, sf::Vector2f newLoc){
	name = "";
	color = newColor;
	setPosition(newLoc);
}

Token::~Token() {

}

void Token::setSize(sf::Vector2i newSize) {
	size = newSize;
}

//Grabs the token with the player. Returns true if successful.
bool Token::grab(std::shared_ptr<Player> grabber){

	if (grabbingPlayer.expired()) {
		grabbingPlayer = grabber;
		return true;
	}
	else {
		return false;
	}

}


void Token::snap() {
	float positionx = getPosition().x;
	float positiony = getPosition().y;
	float newPositionx;
	float newPositiony;

	if (size.x % 2 == 1) {
		newPositionx = ((float)((int)(positionx / 25.f)) * 25.f + 12.5f);
	}
	else {
		newPositionx = ((float)((int)((positionx - 12.5f) / 25.f)) * 25.f + 25.f);
	}

	if (size.y % 2 == 1) {
		newPositiony = ((float)((int)(positiony / 25.f)) * 25.f + 12.5f);
	}
	else {
		newPositiony = ((float)((int)((positiony - 12.5f) / 25.f)) * 25.f + 25.f);
	}

	setPosition(sf::Vector2f(newPositionx, newPositiony));
}