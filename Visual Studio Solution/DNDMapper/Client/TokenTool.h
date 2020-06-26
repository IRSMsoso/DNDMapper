#pragma once
#include "UIElement.h"


class TokenTool: public UIElement{
public:
	TokenTool(sf::RenderWindow*);
	virtual ~TokenTool();

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	virtual void update(sf::Time, sf::Color);



private:
	sf::CircleShape tokenToolButton;
};

