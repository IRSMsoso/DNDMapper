#include "PaintTool.h"

PaintTool::PaintTool(sf::RenderWindow* newWindow) : UIElement(newWindow) {
	paintTexture.loadFromFile("PaintBucket.png");
	flowingAnimation.setSpriteSheet(paintTexture);

	for (int i = 0; i < 16; i++) {
		flowingAnimation.addFrame(sf::IntRect(i * 60, 0, 60, 60));
	}

	paintSprite.setLooped(true);
	paintSprite.setFrameTime(sf::milliseconds(150));
	paintSprite.play(flowingAnimation);

	changeUIRectangle(sf::IntRect(0, 740, 60, 60));
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
}
