#pragma once
#include <SFML/Graphics.hpp>
#include "NetworkManager.h"


const int WINDOWX = 1400;
const int WINDOWY = 800;


class Menu: public sf::Drawable{

public:
	Menu(sf::RenderWindow*, std::vector<std::unique_ptr<Menu>>*, NetworkManager*);
	virtual ~Menu();


	void close() { shouldClose = true; }

	bool getShouldClose() { return shouldClose; }
	
	
	virtual void interpretEvent(sf::Event) = 0;
	virtual void update() = 0;
	void draw(sf::RenderTarget&, sf::RenderStates) const = 0;

	sf::View getCamera() { return camera; }

private:
	bool shouldClose;

protected:
	sf::RenderWindow* window;

	NetworkManager* networkManager;

	sf::View camera;

	std::vector<std::unique_ptr<Menu>>* menuStack;

};
