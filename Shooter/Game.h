#pragma once

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

	void initVars();
	void initWindow();

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

