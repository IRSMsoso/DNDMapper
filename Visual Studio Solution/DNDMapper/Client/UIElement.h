#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ResourceManager.h"

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
	UIElement(ResourceManager* newResourceManager);
	virtual ~UIElement();

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;

	virtual void update(sf::Time, sf::Color) = 0;

	void updateScale(float);


	ToolType::ToolType getToolType();

	bool isClicked(sf::Vector2i);

	sf::IntRect getUIRectangle() { return uiRectangle; }

protected:
	sf::IntRect uiRectangle;
	ToolType::ToolType toolType;

	ResourceManager* resourceManager;
};

