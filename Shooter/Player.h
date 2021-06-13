#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "Bullet.h"

/*
 * Player class for user.
 * 
 * @author MellamoSteve
 */

class Player
{
private:
	//-Stats
	Bullet* bullet;

	//-Basics
	sf::Texture texture;
	sf::Sprite sprite;

	void initVariables();  
	void initSprite();

public:
	//-Constructors
	Player();
	Player(float x, float y, float scaling);
	virtual ~Player();

	//-Getters
	sf::Vector2<float> getPos();

	//-Draw Components
	void update(sf::RenderTarget& target);
	void updateInput();
	void render(sf::RenderTarget& target);
};