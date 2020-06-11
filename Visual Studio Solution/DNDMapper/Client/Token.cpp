#include "Token.h"


Token::Token(sf::Color newColor, sf::Vector2f location){
	circle.setRadius(12.5);
	circle.setFillColor(newColor);
	circle.setOutlineColor(sf::Color::White);
	circle.setOutlineThickness(1);

	hitbox = sf::FloatRect(location, sf::Vector2f(25, 25));


}

Token::~Token(){

}

void Token::setSize(sf::Vector2i newSize){
	hitbox.width = 25 * newSize.x;
	hitbox.height = 25 * newSize.y;

	circle.setScale(newSize.x, newSize.y);
}

void Token::setPosition(sf::Vector2f newLocation){
	circle.setPosition(newLocation);

	hitbox.left = newLocation.x;
	hitbox.top = newLocation.y;
}
