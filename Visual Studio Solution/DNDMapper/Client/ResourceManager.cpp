#include "ResourceManager.h"

Resource* ResourceManager::getResource(std::string resourceName)
{
	for (int i = 0; i < resources.size(); i++) {
		if (resources.at(i)->getResourceID() == resourceName)
			return resources.at(i);
	}

	return nullptr;
}

bool ResourceManager::createResources(std::string fileLocation)
{
	
	std::ifstream input(fileLocation);
	nlohmann::json resourceJson;
	input >> resourceJson;


	int sizeResources = resourceJson["resources"].size();
	for (int i = 0; i < sizeResources; i++) {
		std::string id = resourceJson["resources"].at(i)["id"];
		std::string type = resourceJson["resources"].at(i)["type"];

		if (type == "texture") {
			std::string filename = resourceJson["resources"].at(i)["filename"];
			TextureResource* resource = new TextureResource;
			resource->setResourceID(id);
			resource->loadFromFile("resources/" + filename);
			resources.push_back(resource);
		}

		else if (type == "animation") {
			AnimationResource* resource = new AnimationResource;
			resource->setResourceID(id);
			int sizex = resourceJson["resources"].at(i)["sizex"];
			int sizey = resourceJson["resources"].at(i)["sizey"];
			int numFrames = resourceJson["resources"].at(i)["numframes"];
			std::string textureID = resourceJson["resources"].at(i)["textureid"];
			loadAnimation(*resource, *getTextureResource(textureID), sizex, sizey, numFrames);
			resources.push_back(resource);
		}

		else if (type == "font") {
			FontResource* resource = new FontResource;
			resource->setResourceID(id);
			std::string filename = resourceJson["resources"].at(i)["filename"];
			resource->loadFromFile("resources/" + filename);
			resources.push_back(resource);
		}

	}

	return true;
	
}


void ResourceManager::loadAnimation(Animation& animation, sf::Texture& texture, int sizex, int sizey, int numFrames) {

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
				return;
			}
		}
	}
}