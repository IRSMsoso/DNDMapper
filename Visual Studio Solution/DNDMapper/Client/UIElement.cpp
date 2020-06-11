#include "UIElement.h"

UIElement::UIElement(sf::RenderWindow* newWindow){
	window = newWindow;
	isClickable = false;
	toolType = ToolType::none;
}

UIElement::~UIElement()
{
}

void UIElement::updatePosition(){
	setPosition(window->mapPixelToCoords(sf::Vector2i(uiRectangle.left, uiRectangle.top)));
}

void UIElement::updateScale(float newScale){
	setScale(sf::Vector2f(newScale, newScale));
}

void UIElement::changeUIPosition(sf::Vector2i newPos){
	uiRectangle.left = newPos.x;
	uiRectangle.top = newPos.y;
}

void UIElement::changeUISize(sf::Vector2i newSize){
	uiRectangle.width = newSize.x;
	uiRectangle.height = newSize.y;
}

void UIElement::changeUIRectangle(sf::IntRect newRect){
	uiRectangle = newRect;
}

bool UIElement::getIsClickable(){
	return isClickable;
}

ToolType::ToolType UIElement::getToolType()
{
	return toolType;
}

//Test whether clicking at this mouse location would be clicking on this UI element. Takes in mouse location relative to window.
bool UIElement::isClicked(sf::Vector2i mouseLoc)
{
	if (toolType != ToolType::none) {
		//std::cout << "Testing whether " << uiRectangle.left << ", " << uiRectangle.top << ", " << uiRectangle.width << ", " << uiRectangle.height << " contains " << mouseLoc.x << ", " << mouseLoc.y << std::endl;
		return uiRectangle.contains(mouseLoc);
	}
	//std::cout << "Not Clicked";
	return false;
}
