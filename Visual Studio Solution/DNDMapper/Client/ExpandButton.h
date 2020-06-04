#pragma once
#include <SFML/Graphics.hpp>

class ExpandButton: public sf::Drawable, public sf::Transformable{
public:
	ExpandButton();
	~ExpandButton();

	void onClick(sf::Vector2f);

private:
	sf::RectangleShape buttonRectangle;
};

