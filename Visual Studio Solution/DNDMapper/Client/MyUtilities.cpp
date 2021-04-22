#include "MyUtilities.h"

bool cam::loadAnimation(Animation& animation, sf::Texture& texture, std::string fileName, int sizex, int sizey) {

	if (!texture.loadFromFile(fileName)) {
		//std::cout << "Error with loading file \"" + fileName + "\". Animation will not load.\n";
		return false;
	}

	animation.setSpriteSheet(texture);

	for (int i = 0; i < texture.getSize().y / sizey; i++) {
		for (int w = 0; w < texture.getSize().y / sizex; w++) {
			animation.addFrame(sf::IntRect(w * sizex, i * sizey, sizex, sizey));
		}
	}

	return true;
}

bool cam::loadAnimation(Animation& animation, sf::Texture& texture, std::string fileName, int sizex, int sizey, int numFrames) {
	if (!texture.loadFromFile(fileName)) {
		//std::cout << "Error with loading file \"" + fileName + "\". Animation will not load.\n";
		return false;
	}

	animation.setSpriteSheet(texture);

	int count = 0;

	//std::cout << "Size: " << texture.getSize().x / sizex << ", " << texture.getSize().y / sizey << std::endl;

	for (int i = 0; i < texture.getSize().y / sizey; i++) {
		//std::cout << "I: " << i << std::endl;
		for (int w = 0; w < texture.getSize().x / sizex; w++) {
			if (count < numFrames) {
				animation.addFrame(sf::IntRect(w * sizex, i * sizey, sizex, sizey));
				count++;
			}
			else {
				return true;
			}
		}
	}

	return true;
}

bool cam::isSpriteClicked(AnimatedSprite& sprite, sf::Vector2f mouseLoc) {

	sf::IntRect newGameFrameRect = sprite.getAnimation()->getFrame(0);
	sf::FloatRect newGameRect(sprite.getPosition(), sf::Vector2f(newGameFrameRect.width * sprite.getScale().x, newGameFrameRect.height * sprite.getScale().y));

	return newGameRect.contains(mouseLoc);
}