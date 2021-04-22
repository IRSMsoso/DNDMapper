#pragma once
#include <vector>
#include "ResourceObjects.h"
#include <fstream>
#include "json.hpp"
#include <iostream>
#include "Animation.h"


class ResourceManager {

public:

	Resource* getResource(std::string resourceName);

	bool createResources(std::string fileLocation);

	//Provided for convenience
	TextureResource* getTextureResource(std::string resourceName) { return dynamic_cast<TextureResource*>(getResource(resourceName)); }
	AnimationResource* getAnimationResource(std::string resourceName) { return dynamic_cast<AnimationResource*>(getResource(resourceName)); }
	FontResource* getFontResource(std::string resourceName) { return dynamic_cast<FontResource*>(getResource(resourceName)); }

private:
	void loadAnimation(Animation& animation, sf::Texture& texture, int sizex, int sizey, int numFrames);

	std::vector<Resource*> resources;
};

