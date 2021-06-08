#pragma once

#include <iostream>
#include <vector>
#include <ctime>

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

	//-Logic
	float enemySpawnTimer;
	float enemySpawnTimerMax;		///Might delete
	int maxEnemies;
	int points;


	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//-Game Objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	void initVars();
	void initWindow();
	void initMobs();

public:
	//-Constructor; Destructor
	Game();
	virtual ~Game();

	const bool running() const;
	void update();
	void pollEvents();
	void updateMousePos();
	void updateEnemies();
	void spawnEnemy();
	void render();
	void renderEnemies();
};
