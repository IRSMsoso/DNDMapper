#pragma once
#include <SFML/Graphics.hpp>


class Menu: public sf::Drawable{

public:
	Menu(sf::RenderWindow*);
	virtual ~Menu();


	void close() { shouldClose = true; }
	
	
	virtual void interpretEvent(sf::Event) = 0;
	virtual void update() = 0;
	void draw(sf::RenderTarget&, sf::RenderStates) const = 0;

private:
	bool shouldClose;

protected:
	sf::RenderWindow* window;

};
