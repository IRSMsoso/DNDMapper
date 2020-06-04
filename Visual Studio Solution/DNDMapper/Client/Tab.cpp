#include "Tab.h"

Tab::Tab(sf::RenderWindow* newWindow): UIElement(newWindow){


	changeUIRectangle(sf::IntRect(0, 300, 100, 500));


	shape.setSize(sf::Vector2f(uiRectangle.width, uiRectangle.height));
	shape.setFillColor(sf::Color(55, 55, 55, 200));

}

Tab::~Tab()
{
}

void Tab::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform(); //Combine all the next draws with this object's transform, meaning these next objects can stay at 0, 0.

	target.draw(shape, states);
}
