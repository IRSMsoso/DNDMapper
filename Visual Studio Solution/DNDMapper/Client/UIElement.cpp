#include "UIElement.h"

UIElement::UIElement(){

	toolType = ToolType::none;
}

UIElement::~UIElement()
{
}



ToolType::ToolType UIElement::getToolType()
{
	return toolType;
}


void UIElement::updateScale(float newScale) {
	setScale(sf::Vector2f(newScale, newScale));
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
