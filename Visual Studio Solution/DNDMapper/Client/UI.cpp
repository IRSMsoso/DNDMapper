#include "UI.h"

UI::UI(sf::RenderWindow* newWindow, ResourceManager* newResourceManager){
	window = newWindow;
	resourceManager = newResourceManager;
}

UI::~UI(){

}

void UI::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	for (int i = 0; i < elements.size(); i++) {
		target.draw(*elements.at(i));
	}
}

void UI::setup(bool isDM) {


	elements.push_back(new Tab(resourceManager));
	elements.push_back(new TokenTool(resourceManager));
	if (isDM) {
		elements.push_back(new PaintTool(resourceManager));
		elements.push_back(new FogTool(resourceManager));
	}

}



ToolType::ToolType UI::getToolClicked(sf::Vector2i mouseLoc) {
	for (int i = 0; i < elements.size(); i++) {
		if (elements.at(i)->isClicked(mouseLoc)) {
			return elements.at(i)->getToolType();
		}
	}


	return ToolType::none;
}


void UI::updateElementsPositions() {
	for (int i = 0; i < elements.size(); i++) {
		elements.at(i)->setPosition(window->mapPixelToCoords(sf::Vector2i(elements.at(i)->getUIRectangle().left, elements.at(i)->getUIRectangle().top)));
	}
}

void UI::updateElementsScales(float newScale) {
	for (int i = 0; i < elements.size(); i++) {
		elements.at(i)->updateScale(newScale);
	}
}

void UI::updateElementsAnimations(sf::Time delta, sf::Color newColor){
	for (int i = 0; i < elements.size(); i++) {
		elements.at(i)->update(delta, newColor);
	}
}
