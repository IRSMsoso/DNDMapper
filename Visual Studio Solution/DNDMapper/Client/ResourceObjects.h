#pragma once
#include "Resource.h"
#include <SFML/Graphics.hpp>
#include "Animation.h"

class TextureResource : public sf::Texture, public Resource {};

class FontResource : public sf::Font, public Resource {};

class AnimationResource : public Animation, public Resource {};