#pragma once
#include <SFML/Graphics.hpp>


class UIElement: public sf::Drawable, public sf::Transformable{
public:
	UIElement(sf::RenderWindow*);
	virtual ~UIElement();

	void updatePosition();
	void updateScale(float);

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;

	void changeUIPosition(sf::Vector2i);
	void changeUISize(sf::Vector2i);
	void changeUIRectangle(sf::IntRect);

	bool getIsClickable();

protected:
	sf::RenderWindow* window;
	sf::IntRect uiRectangle;

	bool isClickable;
};

