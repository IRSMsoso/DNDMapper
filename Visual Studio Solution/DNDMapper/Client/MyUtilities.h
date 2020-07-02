#pragma once

#include "AnimatedSprite.h"
#include <iostream>


namespace cam {

	bool loadAnimation(Animation& animation, sf::Texture& texture, std::string fileName, int sizex, int sizey);

	bool loadAnimation(Animation& animation, sf::Texture& texture, std::string fileName, int sizex, int sizey, int numFrames);

	bool isSpriteClicked(AnimatedSprite&, sf::Vector2f);

}
