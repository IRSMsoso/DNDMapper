#include "MainMenu.h"

MainMenu::MainMenu(sf::RenderWindow* newWindow): Menu(newWindow){

	cam::loadAnimation(flameAnimation, flameTexture, "Firesmile.png", 100, 100, 125);
	cam::loadAnimation(fireEyeAnimation, fireEyeTexture, "Fireeye.png", 100, 100, 120);
	
	

	flamesSprite1.setLooped(true);
	flamesSprite2.setLooped(true);
	newGameSprite.setLooped(true);
	flamesSprite1.setFrameTime(sf::milliseconds(20));
	flamesSprite2.setFrameTime(sf::milliseconds(20));
	newGameSprite.setFrameTime(sf::milliseconds(20));

	flamesSprite1.setPosition(700, 200);
	flamesSprite1.setScale(8, 6);

	flamesSprite2.setPosition(700, 200);
	flamesSprite2.setScale(-8, 6);

	newGameSprite.setPosition(250, 0);
	newGameSprite.setScale(4, 4);

	flamesSprite1.play(flameAnimation);
	flamesSprite2.play(flameAnimation);
	newGameSprite.play(fireEyeAnimation);
}

void MainMenu::interpretEvent(sf::Event pollingEvent) {
	if (pollingEvent.type == sf::Event::Closed) {
		window->close();
	}
}

void MainMenu::update() {
	sf::Time updateTime = frameTime.getElapsedTime();

	flamesSprite1.update(updateTime);
	flamesSprite2.update(updateTime);
	newGameSprite.update(updateTime);


	frameTime.restart();
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(flamesSprite1);
	target.draw(flamesSprite2);
	target.draw(newGameSprite);
}
