#include "Tab.h"

Tab::Tab(sf::RenderWindow* newWindow) {


	//changeUIRectangle(sf::IntRect(0, 300, 60, 500));
	uiRectangle = sf::IntRect(0, 300, 60, 500);
	setPosition(0, 300);


	shape.setSize(sf::Vector2f(uiRectangle.width, uiRectangle.height));
	shape.setFillColor(sf::Color(200, 200, 200, 200));

}

Tab::~Tab()
{
}

void Tab::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform(); //Combine all the next draws with this object's transform, meaning these next objects can stay at 0, 0.

	target.draw(shape, states);
}

void Tab::update(sf::Time delta, sf::Color newColor){

}
