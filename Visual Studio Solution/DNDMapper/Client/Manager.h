#pragma once
#include <SFML/Graphics.hpp>
#include "Canvas.h"
#include <iostream>
#include "Camera.h"



const float MAXZOOM = .5;
const float MINZOOM = 1.5;
const float ZOOMSPEED = 0.9;
const float BEADRADIUS = 0.3;

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
	Camera camera;

	//Navigation Tools
	bool isPanning = false;
	sf::Vector2f panLockLoc;
	float zoomFactor;

	//Canvas
	Canvas canvas;

	//Cursors
	sf::Cursor handCursor, defaultCursor;

	//Event interpret
	void interpretEvent(sf::Event);



};
