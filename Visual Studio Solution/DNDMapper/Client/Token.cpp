#include "Token.h"


Token::Token(sf::Color newColor, sf::Vector2f location, sf::Font& newFont) {
	circle.setRadius(10);
	circle.setOrigin(10, 10);
	circle.setFillColor(newColor);

	if (newColor.r * 0.299 + newColor.g * 0.587 + newColor.b * 0.114 > 186) {
		circle.setOutlineColor(sf::Color::Black);
		circle.setOutlineThickness(2);
	}
	else {
		circle.setOutlineColor(sf::Color::White);
		circle.setOutlineThickness(1);
	}

	


	setPosition(location);
	setSize(sf::Vector2i(1, 1));

	
	name.setCharacterSize(20);
	name.setFont(newFont);
	name.setFillColor(sf::Color::White);
	name.setString("Test");

	updateNameLocation();

	std::cout << "Token created at " << location.x << ", " << location.y << std::endl;
}

Token::~Token(){

}

void Token::setSize(sf::Vector2i newSize){
	hitbox.width = 25 * newSize.x;
	hitbox.height = 25 * newSize.y;

	circle.setScale(newSize.x, newSize.y);
}

void Token::setPosition(sf::Vector2f newLocation){
	circle.setPosition(newLocation + sf::Vector2f(12.5, 12.5));

	updateNameLocation();

	hitbox.left = newLocation.x;
	hitbox.top = newLocation.y;
}

bool Token::isClicked(sf::Vector2f worldxy)
{

	return hitbox.contains(worldxy);
}

void Token::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	target.draw(circle);
	target.draw(name);

}

void Token::addNameLetter(char c){
	name.setString(name.getString() + c);
	updateNameLocation();
}

void Token::removeNameLetter(){
	std::string newString = name.getString();
	newString.pop_back();
	name.setString(newString);
	updateNameLocation();
}

void Token::updateNameLocation(){
	sf::FloatRect bounds = name.getGlobalBounds();
	std::cout << "Bounds: " << bounds.left << ", " << bounds.top << ", " << bounds.width << ", " << bounds.height << std::endl;
	name.setOrigin(bounds.width / 2.f, bounds.height);
	name.setPosition(circle.getPosition() + sf::Vector2f(0, -20));
}
