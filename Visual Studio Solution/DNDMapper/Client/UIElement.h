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
	UIElement(sf::RenderWindow*);
	virtual ~UIElement();

	void updatePosition();
	void updateScale(float);

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;

	virtual void update(sf::Time, sf::Color) = 0;

	void changeUIPosition(sf::Vector2i);
	void changeUISize(sf::Vector2i);
	void changeUIRectangle(sf::IntRect);

	bool getIsClickable();

	ToolType::ToolType getToolType();

	bool isClicked(sf::Vector2i);

protected:
	sf::RenderWindow* window;
	sf::IntRect uiRectangle;

	bool isClickable;

	ToolType::ToolType toolType;
};

