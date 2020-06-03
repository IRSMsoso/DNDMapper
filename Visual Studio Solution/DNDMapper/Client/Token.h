#pragma once
#include <SFML/Graphics.hpp>

class Token{
public:
	Token();
	Token(sf::Color);
	~Token();

private:
	sf::Vector2i size;
	sf::CircleShape circle;
};