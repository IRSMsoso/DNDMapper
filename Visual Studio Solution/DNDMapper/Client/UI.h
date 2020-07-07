#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tab.h"
#include "PaintTool.h"
#include "FogTool.h"
#include "TokenTool.h"


class UI: public sf::Transformable, public sf::Drawable{
public:
	UI(sf::RenderWindow*);
	~UI();

	void draw(sf::RenderTarget&, sf::RenderStates) const;


	ToolType::ToolType getToolClicked(sf::Vector2i);


private:
	sf::RenderWindow* window;
	std::vector<UIElement*> elements;
};

