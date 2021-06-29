#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "Bullet.h"

/*
 * Player class for user.
 * 
 * @author Diego Cordova
 */

class Player
{
private:
	//-Stats
	std::vector<Bullet*> bullets;
	float scaling, velocity, sway, hp;
	short shotRate, cooldownCounter, frame,
		maxBullets,	maxHP;
	bool cooldown, alive;

	//-Basics
	sf::Texture tex1;
	sf::Texture tex2;
	//sf::Sprite sprite;

	void initVariables();  
	void initSprite();

public:
	sf::Sprite sprite;
	//-Constructors
	Player();
	Player(float x, float y, float scaling);
	virtual ~Player();

	//-Getters
	float getHP() const;
	short getMaxHP() const;
	bool isAlive() const;
	sf::Vector2<float> getPos() const;
	sf::FloatRect bounds() const;
	std::vector<Bullet*>& getBullets();


	///Delete later
	float see();
	int bulletsCreated(); //Delete quick

	//-Setters

	//-Functions
	bool canAttack();
	float damageDealt(int k);
	void takeDamage(float dmg);
	float getLargestSide();

	//-Draw Components
	void update(sf::RenderTarget& target);
	void updateInput();
	void render(sf::RenderTarget& target);
};
