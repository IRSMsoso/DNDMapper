#pragma once
#include "UIElement.h"
#include <iostream>


class PaintTool: public UIElement{
public:
	PaintTool(sf::RenderWindow*);
	~PaintTool();

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
	sf::Sprite paintSprite;
	sf::Texture paintTexture;
};

