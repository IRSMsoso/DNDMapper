#pragma once
#include <SFML/Graphics.hpp>

const unsigned int WINDOWX = 800;
const unsigned int WINDOWY = 800;

class Tab: public sf::Drawable, public sf::Transformable{
public:
	Tab();
	~Tab();

	void move(float, float);

private:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

	sf::RectangleShape backShape;
	sf::RectangleShape arrowButton;
	std::vector<sf::Sprite> buttons;
};

