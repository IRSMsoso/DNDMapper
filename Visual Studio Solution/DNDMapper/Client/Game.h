#pragma once
#include "Menu.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "UI.h"
#include "ColorWheel.h"
#include "Canvas.h"
#include <fstream>


namespace MouseAction {
	enum MouseAction {
		none,
		painting,
		erasing,
		fogging,
		unfogging,
		colorPicking,
		tokenMoving,
		tokenResizing,
		changingName,
	};
}

enum GameAction {
	newGame,
	joinGame,
	loadGame,
};


const std::string LOWERCASEALPHABET = "abcdefghijklmnopqrstuvwxyz0123456789[];,.'/\\`=- ";
const std::string UPPERCASEALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ)!@#$%^&*({}:<>\"?|~+_ ";
const int ALLOWEDKEYS[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,46,47,48,49,50,51,52,53,54,55,56,57 };

const std::string NUMBERSALPHABET = "01234567890123456789";
const int NUMBERSALLOWEDKEYS[] = { 26,27,28,29,30,31,32,33,34,35,75,76,77,78,79,80,81,82,83,84};

const float MAXZOOM = .5f;
const float MINZOOM = 1.5f; //1.5
const float ZOOMSPEED = 0.9f;
const float BEADRADIUS = 1.0f;
const float CAMERAMOVESPEED = 200.f;



class Game: public Menu {

public:
	Game(sf::RenderWindow*, std::vector<std::unique_ptr<Menu>>*, NetworkManager*, GameAction, std::string filename);
	virtual ~Game();


	void draw(sf::RenderTarget&, sf::RenderStates) const;



	void update();
	void interpretEvent(sf::Event);
	void save();


private:

	//UI Object
	UI ui;

	//Tool Selected
	ToolType::ToolType selectedTool;

	//Navigation Members
	bool isPanning = false;
	sf::Vector2f panLockLoc;
	float zoomFactor;

	//Editing Members
	sf::Color selectedColor;

	//Mouse Action
	MouseAction::MouseAction mouseAction;
	MouseAction::MouseAction previousAction;

	//Color Wheel
	ColorWheel colorWheel;

	//Canvas
	Canvas canvas;

	//Cursors
	sf::Cursor handCursor, defaultCursor;

	//Type of player
	bool isDM;

	//Selected up token
	Token* selectedToken;

	//Resizing original mouse location.
	sf::Vector2f mouseOrigin;
	sf::Vector2i originalSize;

	//FPS Text
	sf::Text fpsText;
	sf::Font algerFont;
	sf::Clock fpsClock;

	//Wrangling Camera.
	void restrictCamera();

	//Have we changed the title to the correct game ID?
	bool gameIDAquired;


	//Saving
	std::string m_filename;
	sf::Clock saveClock;
};
