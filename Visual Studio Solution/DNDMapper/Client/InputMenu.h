#pragma once
#include "Game.h"


class InputMenu: public Menu {

public:
	InputMenu(sf::RenderWindow* newWindow, std::vector<std::unique_ptr<Menu>>* newStack, NetworkManager* newNetworkManager, GameAction purpose);

	virtual ~InputMenu();

	
	void setMenuText(std::string newText) { text.setString(newText); }


	//Menu specific implementations:
	virtual void interpretEvent(sf::Event);
	virtual void update();
	void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
	//std::string text;
	sf::Text text;

	GameAction m_purpose;

	sf::Font algerFont; //Surprisingly OK

};
