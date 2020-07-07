#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

namespace ToolType {
	enum ToolType {
		none, //No need to interact, like Tab
		paintingTool, //DM Only
		fogTool, //DM Only
		tokenTool, //Only DM can use shift to create, otherwise both.
	};
}

class UIElement: public sf::Drawable, public sf::Transformable{
public:
	UIElement();
	virtual ~UIElement();

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;

	virtual void update(sf::Time, sf::Color) = 0;


	ToolType::ToolType getToolType();

	bool isClicked(sf::Vector2i);

protected:
	sf::IntRect uiRectangle;

	ToolType::ToolType toolType;
};

