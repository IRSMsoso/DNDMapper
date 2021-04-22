#include "ColorWheel.h"

ColorWheel::ColorWheel(): fan(sf::PrimitiveType::TriangleFan){

	fan.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color(0, 0, 0, 255)));

	int wedgeNum = VERTEXES / 6;
	for (int i = 0; i < wedgeNum; i++) {
		float normalized = sqrt((float)i / (float)wedgeNum); //0 to 1
		float inverseNormalized = abs(normalized - 1.f); //1 to 0
		fan.append(sf::Vertex(sf::Vector2f(WHEELRADIUS * cosf((2 * PI / (float)VERTEXES)*(float)i), WHEELRADIUS * sinf((2 * PI / (float)VERTEXES)*(float)i)), sf::Color(255, normalized * 255, 0, 255)));

	}
	for (int i = 0; i < wedgeNum; i++) {
		float normalized = sqrt((float)i / (float)wedgeNum);
		//std::cout << normalized << "\n";
		float inverseNormalized = abs(normalized - 1.f);
		//std::cout << inverseNormalized << "\n";
		fan.append(sf::Vertex(sf::Vector2f(WHEELRADIUS * cosf((2 * PI / (float)VERTEXES)*(float)(i + wedgeNum)), WHEELRADIUS * sinf((2 * PI / (float)VERTEXES)*(float)(i + wedgeNum))), sf::Color(inverseNormalized * 255, 255, 0, 255)));
		
	}
	for (int i = 0; i < wedgeNum; i++) {
		float normalized = sqrt((float)i / (float)wedgeNum);
		//std::cout << normalized << "\n";
		float inverseNormalized = abs(normalized - 1.f);
		//std::cout << inverseNormalized << "\n";
		fan.append(sf::Vertex(sf::Vector2f(WHEELRADIUS * cosf((2 * PI / (float)VERTEXES)*(float)(i + wedgeNum * 2)), WHEELRADIUS * sinf((2 * PI / (float)VERTEXES)*(float)(i + wedgeNum * 2))), sf::Color(0, 255, normalized * 255, 255)));

	}
	for (int i = 0; i < wedgeNum; i++) {
		float normalized = sqrt((float)i / (float)wedgeNum); //0 to 1
		float inverseNormalized = abs(normalized - 1.f); //1 to 0
		fan.append(sf::Vertex(sf::Vector2f(WHEELRADIUS * cosf((2 * PI / (float)VERTEXES)*(float)(i + wedgeNum * 3)), WHEELRADIUS * sinf((2 * PI / (float)VERTEXES)*(float)(i + wedgeNum * 3))), sf::Color(0, inverseNormalized * 255, 255, 255)));

	}
	for (int i = 0; i < wedgeNum; i++) {
		float normalized = sqrt((float)i / (float)wedgeNum);
		//std::cout << normalized << "\n";
		float inverseNormalized = abs(normalized - 1.f);
		//std::cout << inverseNormalized << "\n";
		fan.append(sf::Vertex(sf::Vector2f(WHEELRADIUS * cosf((2 * PI / (float)VERTEXES)*(float)(i + wedgeNum * 4)), WHEELRADIUS * sinf((2 * PI / (float)VERTEXES)*(float)(i + wedgeNum * 4))), sf::Color(normalized * 255, 0, 255, 255)));

	}
	for (int i = 0; i < wedgeNum; i++) {
		float normalized = sqrt((float)i / (float)wedgeNum);
		//std::cout << normalized << "\n";
		float inverseNormalized = abs(normalized - 1.f);
		//std::cout << inverseNormalized << "\n";
		fan.append(sf::Vertex(sf::Vector2f(WHEELRADIUS * cosf((2 * PI / (float)VERTEXES)*(float)(i + wedgeNum * 5)), WHEELRADIUS * sinf((2 * PI / (float)VERTEXES)*(float)(i + wedgeNum * 5))), sf::Color(255, 0, inverseNormalized * 255, 255)));

	}
	fan.append(sf::Vertex(fan[1].position, fan[1].color));
	
	//fan[1].color = sf::Color(255, 0, 0, 255);
}

ColorWheel::~ColorWheel(){

}

void ColorWheel::changeMiddle(int delta){
	if ((int)middleScale + delta > 255) {
		middleScale = 255;
	}
	else if ((int)middleScale + delta < 0) {
		middleScale = 0;
		//std::cout << "LESS THAN !!\n";
	}
	else {
		middleScale += delta;
	}
	fan[0].color = sf::Color(middleScale, middleScale, middleScale, 255);
	//std::cout << "Changed Middle Scale to " << (int)middleScale << std::endl;
}

void ColorWheel::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	
	target.draw(fan, states);
}
