#pragma once
#include "UIElement.h"
#include "AnimatedSprite.h"
#include <iostream>


class PaintTool: public UIElement{
public:
	PaintTool(ResourceManager* resourceManager);
	virtual ~PaintTool();

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	virtual void update(sf::Time, sf::Color);

private:
	AnimatedSprite paintSprite;
};

