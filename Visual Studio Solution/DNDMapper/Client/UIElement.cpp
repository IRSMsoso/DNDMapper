#include "UIElement.h"

UIElement::UIElement(sf::RenderWindow* newWindow){
	window = newWindow;
	isClickable = false;
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
