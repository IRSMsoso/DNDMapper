#include "UI.h"

UI::UI(){

}

UI::~UI(){

}

void UI::update(sf::RenderWindow* window, float newScale){
	setPosition(window->mapPixelToCoords(sf::Vector2i(0, 200)));
	setScale(sf::Vector2f(newScale, newScale));
}

void UI::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	// apply the entity's transform -- combine it with the one that was passed by the caller
	states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

	// draw the vertex array
	target.draw(toolTab, states);
}
