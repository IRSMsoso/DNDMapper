#include "Camera.h"

Camera::Camera(): view(sf::FloatRect(0.f, 0.f, WINDOWX*.2, WINDOWY*.2)) {

}

Camera::~Camera(){

}

void Camera::move(float x, float y){
	Transformable::move(x, y);
	view.move(x, y);
}

sf::View* Camera::getView()
{
	return &view;
}

void Camera::draw(sf::RenderTarget & target, sf::RenderStates states) const{
	// apply the entity's transform -- combine it with the one that was passed by the caller
	states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

	// draw the vertex array
	target.draw(toolsPanel, states);
}
