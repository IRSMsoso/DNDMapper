#pragma once
#include "UIElement.h"
#include <iostream>


class FogTool : public UIElement {
public:
	FogTool(sf::RenderWindow*);
	~FogTool();

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

private: 

	sf::Sprite fogSprite;
	sf::Texture fogTexture;
};

