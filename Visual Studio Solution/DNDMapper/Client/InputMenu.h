#pragma once
#include "Game.h"


class InputMenu: public Menu {

public:
	InputMenu(sf::RenderWindow* newWindow, std::vector<std::unique_ptr<Menu>>* newStack, NetworkManager* newNetworkManager, std::unique_ptr<Menu>);

	
	void setMenuText(std::string newText) { text.setString(newText); }


	//Menu specific implementations:
	virtual void interpretEvent(sf::Event);
	virtual void update();
	void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
	//std::string text;
	sf::Text text;
	std::unique_ptr<Menu> nextMenu;
	std::string input;

	sf::Font algerFont; //This is bad.

};
