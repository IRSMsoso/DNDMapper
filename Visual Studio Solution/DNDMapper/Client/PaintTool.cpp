#include "PaintTool.h"

PaintTool::PaintTool(ResourceManager* newResourceManager): UIElement(newResourceManager) {


	paintSprite.setFrameTime(sf::milliseconds(150));
	paintSprite.play(*resourceManager->getAnimationResource("painttoolanimation"));

	uiRectangle = sf::IntRect(0, 740, 60, 60);
	std::cout << "PaintBucket Texture Rect: " << uiRectangle.left << ", " << uiRectangle.top << ", " << uiRectangle.width << ", " << uiRectangle.height << std::endl;


	toolType = ToolType::paintingTool;
}

PaintTool::~PaintTool()
{
}

void PaintTool::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();

	target.draw(paintSprite, states);
}

void PaintTool::update(sf::Time delta, sf::Color newColor){
	paintSprite.update(delta);
	paintSprite.setColor(newColor);
	//std::cout << "Painttool updated\n";
}
