#pragma once
#include "Menu.h"


class InputMenu: public Menu {
	InputMenu(sf::RenderWindow* newWindow, std::vector<std::unique_ptr<Menu>>* newStack, NetworkManager* newNetworkManager, Menu* nextMenu);

	void setNextMenu(Menu* next) { nextMenu = next; }
	void setMenuText(std::string newText) { text = newText; }


	//Menu specific implementations:
	virtual void interpretEvent(sf::Event);
	virtual void update();
	void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
	std::string text;
	Menu* nextMenu;
	std::string input;

};
