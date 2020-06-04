#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tab.h"
#include "PaintTool.h"


class UI: public sf::Transformable{
public:
	UI(sf::RenderWindow*);
	~UI();

	void drawElements();
	void updateElementPositions();
	void updateElementScales(float);

private:
	sf::RenderWindow* window;
	std::vector<UIElement*> elements;
};

