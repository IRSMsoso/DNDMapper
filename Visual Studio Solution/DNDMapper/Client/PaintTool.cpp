#include "PaintTool.h"

PaintTool::PaintTool(sf::RenderWindow* newWindow) : UIElement(newWindow) {
	paintTexture.loadFromFile("PaintBucket.png");
	paintSprite.setTexture(paintTexture);

	changeUIRectangle(sf::IntRect(0, 740, paintSprite.getTextureRect().width, paintSprite.getTextureRect().height));
	std::cout << "PaintBucket Texture Rect: " << uiRectangle.left << ", " << uiRectangle.top << ", " << uiRectangle.width << ", " << uiRectangle.height << std::endl;
}

PaintTool::~PaintTool()
{
}

void PaintTool::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();

	target.draw(paintSprite, states);
}
