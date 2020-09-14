#include "Token.h"



Token::Token(sf::Color newColor, sf::Vector2f newLoc, sf::Uint16 newID){
	name = "";
	color = newColor;
	setPosition(newLoc);
	id = newID;
}

Token::~Token() {

}

void Token::setSize(sf::Vector2i newSize) {
	size = newSize;
}
