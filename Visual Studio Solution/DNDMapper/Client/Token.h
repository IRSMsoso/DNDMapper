#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Token: public sf::Drawable{
public:
	Token(sf::Color, sf::Vector2f, sf::Font&);
	~Token();

	void setSize(sf::Vector2i);

	void setPosition(sf::Vector2f);

	bool isClicked(sf::Vector2f);

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

	void setName(std::string newName) { name.setString(newName); }
	void addNameLetter(char);
	void removeNameLetter();

private:
	sf::CircleShape circle;
	sf::FloatRect hitbox;
	sf::Text name;

	void updateNameLocation();
};