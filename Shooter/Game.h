#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Player.h"
#include "Enemy.h"

/* 
 * "Game Engine". Wrapper Class.
 * 
 * @author MellamoSteve
 */

class Game
{
public:
	///Will be used later for UI
	enum class State
	{
		MainMenu, PauseMenu, Game
	};

private:
	//-Essential Game Variables
	sf::RenderWindow* window;
	sf::VideoMode vidMode;
	sf::Event ev;
	State state;

	//-Logic
	float tileSize;
	float enemySpawnTimer;
	float enemySpawnTimerMax;		///Might delete and instead create a simple script for spawning
	int maxEnemies;
	int points;

	//-Controls, Mouse (Might delete). Might add Keyboard for UI and such
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//-Game Objects
	Player* player;
	//Enemy* enemy;
	std::vector<Enemy*> enemies;

	//-Debug
	sf::Font font;
	sf::Text text;


	//-Basic
	void initVars();
	void initWindow();
	void initMobs();

	//-Debug
	void initText();

public:
	Game();
	~Game();

	//-Getters
	const bool running() const;

	//-Draw Components
	void update();
	void pollEvents();
	void updateMousePos();	//Might delete entirely
	void updateMobs();	//Need reworking
	void render();
	void renderMobs();	//Need to add something to it

	//-Debug
	void updateDebug();
	void renderDebug(sf::RenderTarget& target);
	void drawGrid();
};

