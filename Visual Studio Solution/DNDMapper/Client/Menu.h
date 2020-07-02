#pragma once
#include <SFML/Graphics.hpp>


const int WINDOWX = 1400;
const int WINDOWY = 800;

enum CloseInfo {
	stayOpen,
	close,
	newGame,
	joinGame,
};

class Menu: public sf::Drawable{

public:
	Menu(sf::RenderWindow*);
	virtual ~Menu();


	void close(CloseInfo info) { shouldClose = info; }
	
	
	virtual void interpretEvent(sf::Event) = 0;
	virtual void update() = 0;
	void draw(sf::RenderTarget&, sf::RenderStates) const = 0;

	sf::View getCamera() { return camera; }

private:
	CloseInfo shouldClose;

protected:
	sf::RenderWindow* window;

	sf::View camera;

};
