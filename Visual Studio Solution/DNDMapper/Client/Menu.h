#pragma once
#include <SFML/Graphics.hpp>
#include "NetworkManager.h"
#include "ResourceManager.h"



const int WINDOWX = 1400;
const int WINDOWY = 800;


class Menu: public sf::Drawable{

public:
	struct MenuInfo {
		sf::RenderWindow* window;
		NetworkManager* networkManager;
		std::vector<std::unique_ptr<Menu>>* menuStack;
		ResourceManager* resourceManager;
	};

	Menu(MenuInfo);
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
	MenuInfo getMenuInfo();
	sf::RenderWindow* window;
	std::vector<std::unique_ptr<Menu>>* menuStack;
	NetworkManager* networkManager;
	ResourceManager* resourceManager;

	sf::View camera;


};
