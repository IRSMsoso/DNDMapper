#pragma once
#include <SFML/Graphics.hpp>

class Token{
public:
	Token(sf::Color, sf::Vector2f);
	~Token();

	void setSize(sf::Vector2i);

	void setPosition(sf::Vector2f);

private:
	sf::CircleShape circle;
	sf::FloatRect hitbox;
};