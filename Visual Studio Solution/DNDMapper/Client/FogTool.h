#pragma once
#include "UIElement.h"
#include <iostream>


class FogTool : public UIElement {
public:
	FogTool(ResourceManager* newResourceManager);
	virtual ~FogTool();

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	virtual void update(sf::Time, sf::Color);

private: 

	sf::Sprite fogSprite;
};

