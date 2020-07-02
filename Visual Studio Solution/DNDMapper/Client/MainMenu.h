#pragma once
#include "Menu.h"
#include "AnimatedSprite.h"


class MainMenu: public Menu {

public:

	MainMenu(sf::RenderWindow*);

	virtual void interpretEvent(sf::Event);
	virtual void update();
	void draw(sf::RenderTarget&, sf::RenderStates) const;



private:
	sf::Text newGameText;
	sf::Text openGameText;
	sf::Text joinGameText;

	AnimatedSprite newGameSprite;
	AnimatedSprite openGameSprite;
	AnimatedSprite joinGameText;

};

