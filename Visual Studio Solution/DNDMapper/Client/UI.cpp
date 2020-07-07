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

void UI::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	states.transform *= getTransform();

	for (int i = 0; i < elements.size(); i++) {
		target.draw(*elements.at(i), states);
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
