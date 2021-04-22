#pragma once
#include "UIElement.h"



class Tab: public UIElement{
public:
	Tab(ResourceManager* newResourceManager);
	virtual ~Tab();

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	virtual void update(sf::Time, sf::Color);

private:
	sf::RectangleShape shape; //Don't Transform, drawing does that for us.
};