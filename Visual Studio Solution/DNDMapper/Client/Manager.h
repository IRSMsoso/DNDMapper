#pragma once
#include <SFML/Graphics.hpp>
#include "Canvas.h"
#include <iostream>
#include "UI.h"
#include "ColorWheel.h"


namespace MouseAction {
	enum MouseAction {
		none,
		painting,
		erasing,
		fogging,
		unfogging,
		colorPicking,
		tokenMoving,
		changingName,
	};
}


const std::string LOWERCASEALPHABET = "abcdefghijklmnopqrstuvwxyz0123456789[];,.'/\\`=- ";
const std::string UPPERCASEALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ)!@#$%^&*({}:<>\"?|~+_ ";
const int ALLOWEDKEYS[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,46,47,48,49,50,51,52,53,54,55,56,57};
const int WINDOWX = 1400;
const int WINDOWY = 800;
const float MAXZOOM = .5f;
const float MINZOOM = 1.5f; //1.5
const float ZOOMSPEED = 0.9f;
const float BEADRADIUS = 1.0f;
const float CAMERAMOVESPEED = 200.f;

class Manager{
public:
	Manager(sf::ContextSettings);
	~Manager();

	//Main loop. Called as its own thread.
	void mainLoop();

private:
	//Window
	sf::RenderWindow window;

	//Camera Object
	sf::View camera;

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

	//Event interpret
	void interpretEvent(sf::Event);

	
	//Picked up token
	Token* selectedToken;

	//FPS Text
	sf::Text fpsText;
	sf::Font algerFont;

};
