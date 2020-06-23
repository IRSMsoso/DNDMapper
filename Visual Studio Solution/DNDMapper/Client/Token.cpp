#include "Token.h"


Token::Token(sf::Color newColor, sf::Vector2f location, sf::Font& newFont) {
	circle.setRadius(10);
	circle.setOrigin(10, 10);
	circle.setFillColor(newColor);


	if (newColor.r * 0.299 + newColor.g * 0.587 + newColor.b * 0.114 > 186) {
		circle.setOutlineColor(sf::Color::Black);
		circle.setOutlineThickness(1);
	}
	else {
		circle.setOutlineColor(sf::Color::White);
		circle.setOutlineThickness(.5);
	}

	


	setPosition(location);
	setSize(sf::Vector2i(1, 1));

	beingEdited = false;
	
	nameText.setCharacterSize(20);
	nameText.setFont(newFont);
	nameText.setFillColor(sf::Color::White);
	name = "Test";

	updateName();
	updateNameLocation();

	std::cout << "Token created at " << location.x << ", " << location.y << std::endl;
}

Token::~Token(){

}

void Token::setSize(sf::Vector2i newSize){
	size = newSize;

	circle.setScale(size.x, size.y);

	updateNameLocation();
}

void Token::setPosition(const sf::Vector2f& newLocation){
	Transformable::setPosition(newLocation);

	circle.setPosition(getPosition());

	updateNameLocation();
}

bool Token::isClicked(sf::Vector2f worldxy)
{
	return circle.getGlobalBounds().contains(worldxy);
}

void Token::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	target.draw(circle);
	target.draw(nameText);

}

void Token::addNameLetter(char c){

	if (beingEdited) {
		std::string newString = name;
		newString.pop_back();
		name = newString;
	}

	name += c;

	if (beingEdited)
		name += '|';

	updateName();
	updateNameLocation();
}

void Token::removeNameLetter(){

	if (name.size() > 1) {
		std::string newString = name;
		newString.pop_back();
		newString.pop_back();
		newString.push_back('|');

		name = newString;
	}

	updateName();
	updateNameLocation();
}

void Token::setIsEditing(bool newBeingEdited){
	beingEdited = newBeingEdited;

	if (beingEdited) {
		name += '|';
	}
	else {
		std::string newString = name;
		newString.pop_back();
		name = newString;
	}

	updateName();
	updateNameLocation();
}

void Token::update(){
	if (cursorBlink.getElapsedTime() > sf::seconds(1)) {
		cursorBlink.restart();
		cursorVisible = !cursorVisible;
		updateName();
	}
}

void Token::updateName(){
	if ((!cursorVisible) && beingEdited) {
		std::cout << "Yes\n";
		std::string newName = name;
		if (newName.back() == '|') {
			newName.pop_back();
			nameText.setString(newName);
		}
	}
	else {
		nameText.setString(name);
	}
}

void Token::updateNameLocation(){
	sf::FloatRect bounds = nameText.getGlobalBounds();
	std::cout << "Bounds: " << bounds.left << ", " << bounds.top << ", " << bounds.width << ", " << bounds.height << std::endl;
	nameText.setOrigin(bounds.width / 2.f, nameText.getCharacterSize());
	nameText.setPosition(sf::Vector2f(circle.getPosition().x, circle.getGlobalBounds().top - 15));
}
