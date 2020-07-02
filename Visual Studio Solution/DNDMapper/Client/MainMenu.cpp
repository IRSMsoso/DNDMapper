#include "MainMenu.h"

MainMenu::MainMenu(sf::RenderWindow* newWindow): Menu(newWindow){

	flameTexture.loadFromFile("Firesmile.png");
	flameAnimation.setSpriteSheet(flameTexture);

	int count = 0;
	for (int i = 0; i < flameTexture.getSize().y / 100; i++) {
		for (int w = 0; w < flameTexture.getSize().x / 100; w++) {
			if (count < 125) {
				flameAnimation.addFrame(sf::IntRect(w * 100, i * 100, 100, 100));
				count++;
			}
		}
	}
	
	flamesSprite1.setLooped(true);
	flamesSprite2.setLooped(true);
	flamesSprite1.setFrameTime(sf::milliseconds(20));
	flamesSprite2.setFrameTime(sf::milliseconds(20));

	flamesSprite1.setPosition(700, 200);
	flamesSprite1.setScale(8, 6);

	flamesSprite2.setPosition(700, 200);
	flamesSprite2.setScale(-8, 6);

	flamesSprite1.play(flameAnimation);
	flamesSprite2.play(flameAnimation);
}

void MainMenu::interpretEvent(sf::Event pollingEvent) {
	if (pollingEvent.type == sf::Event::Closed) {
		window->close();
	}
}

void MainMenu::update() {
	flamesSprite1.update(frameTime.getElapsedTime());
	flamesSprite2.update(frameTime.getElapsedTime());



	frameTime.restart();
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(flamesSprite1);
	target.draw(flamesSprite2);
}
