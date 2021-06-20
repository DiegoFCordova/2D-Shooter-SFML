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
	std::vector<Bullet*> bullets;
	float scaling, velocity, sway;
	short shotRate, cooldownCounter, maxBullets;
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
	sf::Vector2<float> getCenter() const;
	sf::FloatRect bounds() const;
	std::vector<Bullet*>& getBullets();


	float see();
	int bulletsCreated(); //Delete quick

	//-Setters

	//-Functions
	bool canAttack();
	float damageDealt(int k);

	//-Draw Components
	void update(sf::RenderTarget& target);
	void updateInput();
	void render(sf::RenderTarget& target);
};
