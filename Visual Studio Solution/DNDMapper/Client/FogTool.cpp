#include "FogTool.h"

FogTool::FogTool(ResourceManager* newResourceManager): UIElement(newResourceManager) {
	fogSprite.setTexture(*resourceManager->getTextureResource("fogtooltexture"));

	uiRectangle = sf::IntRect(0, 680, fogSprite.getTextureRect().width, fogSprite.getTextureRect().height);
	std::cout << "FogTool Texture Rect: " << uiRectangle.left << ", " << uiRectangle.top << ", " << uiRectangle.width << ", " << uiRectangle.height << std::endl;


	toolType = ToolType::fogTool;
}


FogTool::~FogTool()
{
}

void FogTool::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(fogSprite, states);
}

void FogTool::update(sf::Time delta, sf::Color newColor) {

}