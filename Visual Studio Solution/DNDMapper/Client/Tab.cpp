#include "Tab.h"

Tab::Tab() {
	backShape.setSize(sf::Vector2f((WINDOWX *.2) / 3, (WINDOWX * .2) / 2));
	backShape.setFillColor(sf::Color(100, 100, 100, 255));
}

Tab::~Tab(){

}

void Tab::setScale(sf::Vector2f factor){
	Transformable::setScale(factor);
	backShape.setScale(factor);
}


void Tab::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	// apply the entity's transform -- combine it with the one that was passed by the caller
	//states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

	// draw the vertex array
	target.draw(backShape, states);
}

void Tab::move(float x, float y){
	std::cout << "toolPanel Moved " << x << ", " << y << std::endl;
	sf::Transformable::move(x, y);
	backShape.move(x, y);
}
