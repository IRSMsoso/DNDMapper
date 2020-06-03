#include "Camera.h"

Camera::Camera(): view(sf::FloatRect(0.f, 0.f, WINDOWX*.2, WINDOWY*.2)) {

}

Camera::~Camera(){

}

void Camera::move(float x, float y){
	Transformable::move(x, y);
	view.move(x, y);
	toolsPanel.move(x, y);
}


sf::View Camera::getView()
{
	return view;
}

void Camera::zoom(float amount){
	view.zoom(amount);
	sf::Vector2f viewTLPos = view.getCenter() - (view.getSize() / 2.f);
	sf::Vector2f diff = viewTLPos - toolsPanel.getPosition();
	std::cout << "Camera: " << getPosition().x << ", " << getPosition().y << std::endl;
	std::cout << "Toolpanel: " << toolsPanel.getPosition().x << ", " << toolsPanel.getPosition().y << std::endl;
	toolsPanel.move(diff.x, diff.y);
	toolsPanel.setScale(sf::Vector2f(toolsPanel.getScale().x * amount, toolsPanel.getScale().y * amount));
}

void Camera::draw(sf::RenderTarget & target, sf::RenderStates states) const{
	// apply the entity's transform -- combine it with the one that was passed by the caller
	//states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

	// draw the vertex array
	target.draw(toolsPanel, states);
}
