#include "MainMenu.h"

MainMenu::MainMenu(sf::RenderWindow* newWindow, std::vector<std::unique_ptr<Menu>>* newStack, NetworkManager* newNetworkManager): Menu(newWindow, newStack, newNetworkManager){

	//Animated Sprites
	cam::loadAnimation(flameAnimation, flameTexture, "Firesmile.png", 100, 100, 125);
	cam::loadAnimation(fireEyeAnimation, fireEyeTexture, "Fireeye.png", 41, 45, 120);
	cam::loadAnimation(fireJoinAnimation, fireJoinTexture, "fireplay.png", 39, 45, 120);
	cam::loadAnimation(connectedAnimation, connectedTexture, "Heartrate Monitor.png", 20, 11, 29);
	cam::loadAnimation(disconnectedAnimation, disconnectedTexture, "Heartrate Flatline.png", 20, 11, 30);
	

	flamesSprite1.setLooped(true);
	flamesSprite2.setLooped(true);
	newGameSprite.setLooped(true);
	joinGameSprite.setLooped(true);
	connectedSprite.setLooped(true);
	disconnectedSprite.setLooped(true);
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

	newGameSprite.setPosition(350, 100);
	newGameSprite.setScale(4, 4);

	joinGameSprite.setPosition(900, 100);
	joinGameSprite.setScale(4, 4);

	connectedSprite.setPosition(0, 0);
	connectedSprite.setScale(4, 4);
	
	disconnectedSprite.setPosition(0, 0);
	disconnectedSprite.setScale(4, 4);

	flamesSprite1.play(flameAnimation);
	flamesSprite2.play(flameAnimation);
	newGameSprite.play(fireEyeAnimation);
	joinGameSprite.play(fireJoinAnimation);
	connectedSprite.play(connectedAnimation);
	disconnectedSprite.play(disconnectedAnimation);


	//Cursors
	normalCursor.loadFromSystem(sf::Cursor::Type::Arrow);
	hoveringCursor.loadFromSystem(sf::Cursor::Type::Hand);


	//Networking
	isConnected = false;
	isVersionCorrect = false;

}

void MainMenu::interpretEvent(sf::Event pollingEvent) {
	switch (pollingEvent.type) {
	case sf::Event::Closed:
		window->close();
		break;

	case sf::Event::MouseMoved:

		if (cam::isSpriteClicked(newGameSprite, window->mapPixelToCoords(sf::Mouse::getPosition(*window))) || cam::isSpriteClicked(joinGameSprite, window->mapPixelToCoords(sf::Mouse::getPosition(*window)))) {
			if (isConnected && isVersionCorrect) {
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
			if (isConnected && isVersionCorrect) {
				Command outCommand;
				outCommand.type = CommandType::CreateGame;
				networkManager->sendCommand(outCommand);
				menuStack->push_back(std::unique_ptr<InputMenu>(new InputMenu(window, menuStack, networkManager, GameAction::newGame)));
			}
		}

		if (cam::isSpriteClicked(joinGameSprite, window->mapPixelToCoords(sf::Mouse::getPosition(*window)))) {
			if (isConnected && isVersionCorrect) {
				menuStack->push_back(std::unique_ptr<InputMenu>(new InputMenu(window, menuStack, networkManager, GameAction::loadGame))); //Load game just for now
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
			isVersionCorrect = networkManager->getIsVersionCorrect();

		}
		connectionUpdateClock.restart();
	}

}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(flamesSprite1);
	target.draw(flamesSprite2);
	target.draw(newGameSprite);
	target.draw(joinGameSprite);
	if (isConnected) {
		target.draw(connectedSprite);
	}
	else {
		target.draw(disconnectedSprite);
	}
}
