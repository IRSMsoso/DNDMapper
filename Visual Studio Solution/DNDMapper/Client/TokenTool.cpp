#include "TokenTool.h"

TokenTool::TokenTool(sf::RenderWindow* newWindow) {
	tokenToolButton.setRadius(20);
	tokenToolButton.move(10, 10);
	tokenToolButton.setOutlineColor(sf::Color::White);
	tokenToolButton.setFillColor(sf::Color::Black);
	tokenToolButton.setOutlineThickness(2);
	
	uiRectangle = sf::IntRect(0, 620, 60, 60);


	toolType = ToolType::tokenTool;

}

TokenTool::~TokenTool()
{
}

void TokenTool::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	
	states.transform *= getTransform();

	//std::cout << "Fog Tool Location: " << getPosition().x << ", " << getPosition().y << std::endl;

	target.draw(tokenToolButton, states);
}

void TokenTool::update(sf::Time delta, sf::Color newColor){

}
