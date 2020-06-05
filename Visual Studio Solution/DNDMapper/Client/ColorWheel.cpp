#include "ColorWheel.h"

ColorWheel::ColorWheel(): fan(sf::PrimitiveType::TriangleFan){

	fan.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color(0, 0, 0, 255)));
	for (int i = 0; i < VERTEXES; i++) {
		fan.append(sf::Vertex(sf::Vector2f(RADIUS * cosf((2 * PI / (float)VERTEXES)*(float)i), RADIUS * sinf((2 * PI / (float)VERTEXES)*(float)i)), sf::Color(255, 0, 0, 255)));
	}
	fan.append(sf::Vertex(fan[1].position, fan[1].color));
	
}

ColorWheel::~ColorWheel(){

}

void ColorWheel::changeMiddle(int delta){
	if ((int)middleScale + delta > 255) {
		middleScale = 255;
	}
	else if ((int)middleScale + delta < 0) {
		middleScale = 0;
		std::cout << "LESS THAN !!\n";
	}
	else {
		middleScale += delta;
	}
	fan[0].color = sf::Color(middleScale, middleScale, middleScale, 255);
	std::cout << "Changed Middle Scale to " << (int)middleScale << std::endl;
}

void ColorWheel::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	
	target.draw(fan, states);
}
