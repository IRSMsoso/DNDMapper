#pragma once
#include "Menu.h"
#include "AnimatedSprite.h"
#include "MyUtilities.h"
#include "Game.h"
#include "InputMenu.h"


class MainMenu: public Menu {

public:

	MainMenu(MenuInfo menuInfo);

	virtual void interpretEvent(sf::Event);
	virtual void update();
	void draw(sf::RenderTarget&, sf::RenderStates) const;



private:
	
	
	// Animation for objects for main menu 

	AnimatedSprite newGameSprite;
	AnimatedSprite joinGameSprite;
	AnimatedSprite flamesSprite1;
	AnimatedSprite flamesSprite2;
	AnimatedSprite connectedSprite;
	AnimatedSprite disconnectedSprite;
	AnimatedSprite loadGameSprite;


	Animation fireJoinAnimation;
	Animation fireLoadAnimation;
	Animation flameAnimation;
	Animation connectedAnimation;
	Animation disconnectedAnimation;
	sf::Texture flameTexture;
	sf::Texture fireLoadTexture;
	sf::Texture fireJoinTexture;
	sf::Texture connectedTexture;
	sf::Texture disconnectedTexture;


	sf::Clock frameTime;

	sf::Cursor hoveringCursor;
	sf::Cursor normalCursor;


	//Networking
	bool isConnected;
	sf::Clock connectionUpdateClock;

};

