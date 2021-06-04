#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/* 
 * "Game Engine". Wrapper Class.
 */

class Game
{
private:
	//-Variables
	sf::RenderWindow* window;
	sf::VideoMode vidMode;
	sf::Event ev;

	//Game Objects
	sf::RectangleShape mob;

	void initVars();
	void initWindow();
	void initMobs();

public:
	//-Constructor; Destructor
	Game();
	virtual ~Game();

	//-Accessors
	const bool running() const;

	//-Functions
	void pollEvents();
	void update();
	void render();
};

