#include "FogTool.h"

FogTool::FogTool(sf::RenderWindow* newWindow) : UIElement(newWindow) {
	fogTexture.loadFromFile("FogTool.png");
	fogSprite.setTexture(fogTexture);

	changeUIRectangle(sf::IntRect(0, 680, fogSprite.getTextureRect().width, fogSprite.getTextureRect().height));
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
