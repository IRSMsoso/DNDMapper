#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tab.h"
#include "PaintTool.h"
#include "FogTool.h"


class UI: public sf::Transformable{
public:
	UI(sf::RenderWindow*);
	~UI();

	void drawElements();
	void updateElementPositions();
	void updateElementScales(float);

	ToolType getToolClicked(sf::Vector2i);

private:
	sf::RenderWindow* window;
	std::vector<UIElement*> elements;
};

