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

#include "DeathAni.h"
#include "Player.h"
#include "Enemy.h"
#include "Star.h"
#include "UI.h"

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
		Opening, MainMenu, PauseMenu, Game, GameOver
	};

private:
	//-Essential Game Variables
	sf::RenderWindow* window;
	sf::VideoMode vidMode;
	sf::Event ev;
	GameState state;

	//-Logic
	///sf::Clock clock;
	float tileSize;
	int enemySpawnRate,
		maxEnemies, score,
		openingFrame, ticks, reviveTicks;
	bool debug;

	//-Game Objects
	Player* player;
	std::vector<Enemy*> enemies;
	std::vector<DeathAni*> death;
	std::vector<Star*> stars;
	sf::RectangleShape blackscreen;

	sf::Texture titleTex;
	sf::Sprite title;
	UI* ui;

	//-Debug
	sf::Font font;
	sf::Text text, temp;

	//-Basic
	void initVars();
	void initWindow();
	void initMobs();
	void initStars();
	void resetMobs();
	void refreshUI();

	//-Debug
	void initText();

public:
	///float dt;

	Game();
	~Game();

	//-Getters
	const bool running() const;

	//-Functions
	template <typename T>
	void deleteMob(std::vector<T*>* mobArray, T* mob, int index);
	void deleteBulletOf(Mob* mob, int index);
	bool skipOpening();

	//-Draw Components
	void update();
	void updateTimer();
	void reviveSequence();
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

	///void updateClock();
};

