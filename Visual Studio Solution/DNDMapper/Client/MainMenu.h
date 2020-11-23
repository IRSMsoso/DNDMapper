#pragma once
#include "Menu.h"
#include "AnimatedSprite.h"
#include "MyUtilities.h"
#include "Game.h"


class MainMenu: public Menu {

public:

	MainMenu(sf::RenderWindow*, std::vector<std::unique_ptr<Menu>>*, NetworkManager*);

	virtual void interpretEvent(sf::Event);
	virtual void update();
	void draw(sf::RenderTarget&, sf::RenderStates) const;



private:
	sf::Text newGameText;
	sf::Text openGameText;
	sf::Text joinGameText;

	AnimatedSprite newGameSprite;
	sf::Sprite openGameSprite;
	AnimatedSprite joinGameSprite;
	AnimatedSprite flamesSprite1;
	AnimatedSprite flamesSprite2;

	Animation fireEyeAnimation;
	Animation fireJoinAnimation;
	Animation flameAnimation;
	sf::Texture flameTexture;
	sf::Texture fireEyeTexture;
	sf::Texture fireJoinTexture;


	sf::Clock frameTime;

	sf::Cursor hoveringCursor;
	sf::Cursor normalCursor;


	//Networking
	bool isConnected;
	bool isVersionCorrect;
	sf::Clock connectionUpdateClock;

};

