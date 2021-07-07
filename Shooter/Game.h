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
#include "DeathAni.h"
#include "Star.h"

/* 
 * "Game Engine". Wrapper Class.
 * 
 * @author Diego Cordova
 */

class Game
{
public:
	///Will be used later for UI
	enum class GameState
	{
		MainMenu, PauseMenu, Game
	};

private:
	//-Essential Game Variables
	sf::RenderWindow* window;
	sf::VideoMode vidMode;
	sf::Event ev;
	GameState state;

	//-Logic
	float tileSize;
	int enemySpawnTimer, enemySpawnRate,
		maxEnemies, score, starsSpeed;
	bool debug;

	//-Game Objects
	Player* player;
	std::vector<Enemy*> enemies;
	std::vector<DeathAni*> death;
	std::vector<Star*> stars;

	//-Debug
	sf::Font font;
	sf::Text text;

	//-Basic
	void initVars();
	void initWindow();
	void initMobs();
	void initStars();

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
	void updateMobs();	///Need reworking
	void updateStars();	///Might rename to UI when other stuffs are added
	
	void render();
	void renderMobs();	///Need to add something to it
	void renderStars();

	//-Debug
	void updateDebug();
	
	void renderDebug(sf::RenderTarget& target);
	void drawGrid();
};

