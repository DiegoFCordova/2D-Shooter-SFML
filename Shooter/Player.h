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
public:
	enum class Direction
	{
		Left, Right, Neutral
	};

private:
	//-Stats
	std::vector<Bullet*> bullets;
	Direction dir;
	float scaling;
	float velocity;
	float sway;
	short shotRate;
	short counter;
	bool cooldown;

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
	sf::Vector2<float> getPos() const;
	sf::FloatRect bounds() const;
	std::vector<Bullet*>& getBullets();


	float see();
	int bulletsCreated(); //Delete quick

	//-Setters

	//-Functions
	bool canAttack();

	//-Draw Components
	void update(sf::RenderTarget& target);
	void updateInput();
	void render(sf::RenderTarget& target);
};
