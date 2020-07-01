#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Token: public sf::Drawable, public sf::Transformable {
public:
	Token(sf::Color, sf::Vector2f, sf::Font&);
	~Token();

	void setSize(sf::Vector2i);
	sf::Vector2i getSize() { return size; }

	void setPosition(const sf::Vector2f&);

	bool isClicked(sf::Vector2f);

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

	void setName(std::string newName) { name = newName; updateName(); }
	void addNameLetter(char);
	void removeNameLetter();
	void setIsEditing(bool);

	void update();

	sf::Uint16 getID() { return id; }
	void setID(sf::Uint16 newID) { id = newID; }


	void snap();

private:
	sf::CircleShape circle;
	sf::Vector2i size;
	std::string name;
	sf::Text nameText;
	sf::Uint16 id;

	bool beingEdited;
	bool cursorVisible;

	void updateName();
	void updateNameLocation();

	sf::Clock cursorBlink;
};