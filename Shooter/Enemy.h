#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "Bullet.h"

/*
 * Base enemy class for mobs.
 * 
 * @author Diego Cordova
 */

class Enemy
{
	//Make Enum for Variation sprite
public:
	enum class Type
	{
		Common, Strong, Fast
	};

	/*
 	 * States for easier enemy control:
	 *	  Alive: Enemy can move and shoot.
	 *	  WaitingDisposal: Only bullets move.
	 *	  Death: If no more bullets in the field and enemy is death.
	 */
	enum class State
	{
		Alive, WaitingDisposal, Death
	};

private:
	///Mostly for initial, don't forget there is already x and y in sprite
	std::vector<Bullet*> bullets;
	State state;
	float hp;
	float xTarget, yTarget;
	int tick, maxHP, maxBullets;

	sf::Texture texture;
	sf::Sprite sprite;

	void initVariables();
	void initSprite();

public:
	Enemy();
	Enemy(float x, float y);
	~Enemy();

	//-Getters
	sf::Vector2<float> getPos() const;
	sf::FloatRect bounds() const;
	std::vector<Bullet*>& getBullets();
	bool isAlive() const;

	//-Functions
	float getLargestSide();
	float damageDealt(int k);
	int bulletsInScreen();
	void attack(float x, float y);
	void takeDamage(float dmg);
	void setWaitForDisposal();

	//-Draw Components
	void update(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);


};

