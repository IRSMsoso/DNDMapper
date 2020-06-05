#pragma once
#include "UIElement.h"



class Tab: public UIElement{
public:
	Tab(sf::RenderWindow*);
	~Tab();

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
	sf::RectangleShape shape; //Don't Transform, drawing does that for us.
};