#pragma once
#include "Game.h"


class InputMenu: public Menu {

public:
	InputMenu(MenuInfo menuInfo, GameAction purpose);

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

};
