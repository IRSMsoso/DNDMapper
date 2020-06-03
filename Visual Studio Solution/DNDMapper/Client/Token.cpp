#include "Token.h"

Token::Token(){
	circle.setRadius(2.5);
}

Token::Token(sf::Color newColor){
	Token();
	circle.setFillColor(newColor);
}

Token::~Token(){

}
