#pragma once
#include "Tab.h"

class UI: public sf::Transformable, public sf::Drawable{
public:

	UI();
	~UI();

	void update(sf::RenderWindow*, float);

private:

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;


	//UI Elements
	Tab toolTab;
};

