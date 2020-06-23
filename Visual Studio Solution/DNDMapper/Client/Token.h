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

	void setName(std::string newName) { name = newName; updateName(); }
	void addNameLetter(char);
	void removeNameLetter();
	void setIsEditing(bool);

	void update();

private:
	sf::CircleShape circle;
	sf::FloatRect hitbox;
	std::string name;
	sf::Text nameText;

	bool beingEdited;
	bool cursorVisible;

	void updateName();
	void updateNameLocation();

	sf::Clock cursorBlink;
};