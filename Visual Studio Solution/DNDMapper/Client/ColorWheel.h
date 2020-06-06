#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

const double PI = atan(1) * 4;
const unsigned int WHEELRADIUS = 100;
const unsigned int VERTEXES = 40;

class ColorWheel: public sf::Transformable, public sf::Drawable{
public:
	ColorWheel();
	~ColorWheel();

	void changeMiddle(int);

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
	sf::VertexArray fan;
	sf::Uint8 middleScale;
};

