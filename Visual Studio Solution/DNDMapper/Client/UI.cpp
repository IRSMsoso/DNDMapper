#include "UI.h"

UI::UI(sf::RenderWindow* newWindow){
	window = newWindow;
	
	elements.push_back(new Tab(window));
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
