#include "MainMenu.h"

MainMenu::MainMenu(MenuInfo menuInfo): Menu(menuInfo){
	
	
	//Set frame timings for all the animations.
	loadGameSprite.setFrameTime(sf::milliseconds(20));
	flamesSprite1.setFrameTime(sf::milliseconds(20));
	flamesSprite2.setFrameTime(sf::milliseconds(20));
	newGameSprite.setFrameTime(sf::milliseconds(20));
	joinGameSprite.setFrameTime(sf::milliseconds(20));
	connectedSprite.setFrameTime(sf::milliseconds(50));
	disconnectedSprite.setFrameTime(sf::milliseconds(50));

	flamesSprite1.setPosition(700, 200);
	flamesSprite1.setScale(8, 6);

	flamesSprite2.setPosition(700, 200);
	flamesSprite2.setScale(-8, 6);

	newGameSprite.setPosition(450, 300);
	newGameSprite.setScale(4, 4);

	joinGameSprite.setPosition(800, 300);
	joinGameSprite.setScale(4, 4);

	loadGameSprite.setPosition(800, 0);
	loadGameSprite.setScale(-3, 3);

	connectedSprite.setPosition(0, 0);
	connectedSprite.setScale(4, 4);
	
	disconnectedSprite.setPosition(0, 0);
	disconnectedSprite.setScale(4, 4);

	flamesSprite1.play(*resourceManager->getAnimationResource("firesmileanimation"));
	flamesSprite2.play(*resourceManager->getAnimationResource("firesmileanimation"));
	newGameSprite.play(*resourceManager->getAnimationResource("newgameanimation"));
	joinGameSprite.play(*resourceManager->getAnimationResource("joingameanimation"));
	connectedSprite.play(*resourceManager->getAnimationResource("connectedanimation"));
	disconnectedSprite.play(*resourceManager->getAnimationResource("disconnectedanimation"));
	loadGameSprite.play(*resourceManager->getAnimationResource("loadgameanimation"));


	//Cursors
	normalCursor.loadFromSystem(sf::Cursor::Type::Arrow);
	hoveringCursor.loadFromSystem(sf::Cursor::Type::Hand);


	//Networking
	isConnected = false;

}

void MainMenu::interpretEvent(sf::Event pollingEvent) {
	switch (pollingEvent.type) {
	case sf::Event::Closed:
		window->close();
		break;

	case sf::Event::MouseMoved:

		if (cam::isSpriteClicked(newGameSprite, window->mapPixelToCoords(sf::Mouse::getPosition(*window))) || cam::isSpriteClicked(joinGameSprite, window->mapPixelToCoords(sf::Mouse::getPosition(*window))) || cam::isSpriteClicked(loadGameSprite, window->mapPixelToCoords(sf::Mouse::getPosition(*window)))) {
			if (isConnected) {
				window->setMouseCursor(hoveringCursor);
			}
			//std::cout << "isConnected: " << isConnected << ", isVersionCorrect: " << isVersionCorrect << std::endl;
		}

		else if (cam::isSpriteClicked(connectedSprite, window->mapPixelToCoords(sf::Mouse::getPosition(*window)))) {
			if (!isConnected) {
				window->setMouseCursor(hoveringCursor);
			}
		}

		else {
			window->setMouseCursor(normalCursor);
		}
		break;


	case sf::Event::MouseButtonPressed:

		if (cam::isSpriteClicked(newGameSprite, window->mapPixelToCoords(sf::Mouse::getPosition(*window)))) {
			if (isConnected) {
				menuStack->push_back(std::unique_ptr<InputMenu>(new InputMenu(getMenuInfo(), GameAction::newGame)));
			}
		}

		if (cam::isSpriteClicked(joinGameSprite, window->mapPixelToCoords(sf::Mouse::getPosition(*window)))) {
			if (isConnected) {
				menuStack->push_back(std::unique_ptr<InputMenu>(new InputMenu(getMenuInfo(), GameAction::joinGame))); //Load game just for now
			}
		}

		if (cam::isSpriteClicked(loadGameSprite, window->mapPixelToCoords(sf::Mouse::getPosition(*window)))) {
			if (isConnected) {
				menuStack->push_back(std::unique_ptr<InputMenu>(new InputMenu(getMenuInfo(), GameAction::loadGame))); //Load game just for now
			}
		}

		if (cam::isSpriteClicked(connectedSprite, window->mapPixelToCoords(sf::Mouse::getPosition(*window)))) {
			if (!isConnected) {
				networkManager->startConnect("173.24.79.165");
			}
		}



		break;

	}

}

void MainMenu::update() {
	//Animated Sprites Update
	sf::Time updateTime = frameTime.getElapsedTime();

	flamesSprite1.update(updateTime);
	flamesSprite2.update(updateTime);
	newGameSprite.update(updateTime);
	joinGameSprite.update(updateTime);
	loadGameSprite.update(updateTime);
	if (isConnected) {
		connectedSprite.update(updateTime);
	}
	else {
		disconnectedSprite.update(updateTime);
	}

	frameTime.restart();


	//Networking Update
	if (connectionUpdateClock.getElapsedTime().asSeconds() >= 1) {
		std::cout << "Updated Networking Info.\n";
		isConnected = networkManager->getIsConnected();
		if (isConnected) {
			//Get Version Info.
			std::cout << "Got Version Info\n";

		}
		connectionUpdateClock.restart();
	}

}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(flamesSprite1);
	target.draw(flamesSprite2);
	target.draw(newGameSprite);
	target.draw(joinGameSprite);
	target.draw(loadGameSprite);
	if (isConnected) {
		target.draw(connectedSprite);
	}
	else {
		target.draw(disconnectedSprite);
	}
}
