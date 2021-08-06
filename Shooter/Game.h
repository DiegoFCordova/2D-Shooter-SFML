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
#include "DT.h"

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
	/* For these vectors, the x represents the limit for a
	 * specific action to happen, y is the counter that
	 * will increase depending on the fps.
	 */
	sf::Vector2f openingFrame, enemySpawnRate, score;
	float tileSize, ticks, reviveTicks;
	int maxEnemies;
	short fpsID;
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
	Game();
	~Game();

	//-Getters
	const bool running() const;

	//-Functions
	template <typename T>
	void deleteMob(std::vector<T*>* mobArray, T* mob, int index);
	void deleteBulletOf(Mob* mob, int index);
	bool skipOpening();
	void diffChanges(UI::Difficulty diff);

	//-Draw Components
	void update();
	void updateTimer();
	void reviveSequence();
	void pollEvents();
	void updateMobs();
	void updateStars();
		
	void render();
	void renderMobs();
	void renderStars();

	//-Debug
	void updateDebug();
	
	void renderDebug(sf::RenderTarget& target);
	void drawGrid();
};

