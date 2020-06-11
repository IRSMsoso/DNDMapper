#pragma once
#include "UIElement.h"


class TokenTool: public UIElement{
public:
	TokenTool(sf::RenderWindow*);
	~TokenTool();

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
	sf::CircleShape tokenToolButton;
};

