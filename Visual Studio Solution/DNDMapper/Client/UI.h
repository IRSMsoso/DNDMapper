#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tab.h"
#include "PaintTool.h"
#include "FogTool.h"
#include "TokenTool.h"


class UI: public sf::Drawable {
public:
	UI(sf::RenderWindow*);
	~UI();

	void draw(sf::RenderTarget&, sf::RenderStates) const;
	void setup(bool isDM);


	ToolType::ToolType getToolClicked(sf::Vector2i);

	void updateElementsPositions();
	void updateElementsScales(float);
	void updateElementsAnimations(sf::Time delta, sf::Color newColor);



private:
	sf::RenderWindow* window;
	std::vector<UIElement*> elements;
};

