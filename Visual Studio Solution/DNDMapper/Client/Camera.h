#pragma once
#include <SFML/Graphics.hpp>
#include "Tab.h"

class Camera: public sf::Transformable, public sf::Drawable{
public:
	Camera();
	~Camera();

	void move(float, float);

	sf::View* getView();

private:

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

	Tab toolsPanel;

	sf::View view;
};

