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

	};
}


const int WINDOWX = 1400;
const int WINDOWY = 800;
const float MAXZOOM = .5;
const float MINZOOM = 1.5;
const float ZOOMSPEED = 0.9;
const float BEADRADIUS = 1.0;

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

	//Fog Cloud
	sf::Texture fogCloudTexture;

};
