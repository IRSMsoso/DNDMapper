#pragma once
#include "Menu.h"
#include "AnimatedSprite.h"
#include "MyUtilities.h"
#include "Game.h"
#include "InputMenu.h"


class MainMenu: public Menu {

public:

	MainMenu(sf::RenderWindow*, std::vector<std::unique_ptr<Menu>>*, NetworkManager*);

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


	Animation fireEyeAnimation;
	Animation fireJoinAnimation;
	Animation flameAnimation;
	Animation connectedAnimation;
	Animation disconnectedAnimation;
	sf::Texture flameTexture;
	sf::Texture fireEyeTexture;
	sf::Texture fireJoinTexture;
	sf::Texture connectedTexture;
	sf::Texture disconnectedTexture;


	sf::Clock frameTime;

	sf::Cursor hoveringCursor;
	sf::Cursor normalCursor;


	//Networking
	bool isConnected;
	bool isVersionCorrect;
	sf::Clock connectionUpdateClock;

};

