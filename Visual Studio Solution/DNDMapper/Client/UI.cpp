#include "UI.h"

UI::UI(sf::RenderWindow* newWindow){
	window = newWindow;
	
	elements.push_back(new Tab(window));
	elements.push_back(new PaintTool(window));
	elements.push_back(new FogTool(window));
	elements.push_back(new TokenTool(window));
}

UI::~UI(){

}

void UI::drawElements(){
	for (int i = 0; i < elements.size(); i++) {
		window->draw(*elements.at(i));
	}
}

void UI::updateElementPositions(){
	for (int i = 0; i < elements.size(); i++) {
		elements.at(i)->updatePosition();
	}
}

void UI::updateElementScales(float newScale){
	for (int i = 0; i < elements.size(); i++) {
		elements.at(i)->updateScale(newScale);
	}
}

void UI::updateElementsAnimations(sf::Time delta, sf::Color newColor){
	for (int i = 0; i < elements.size(); i++) {
		elements.at(i)->update(delta, newColor);
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
