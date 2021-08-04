#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "DT.h"

/*
 * Some classes had a lot of methods in common, so I made
 * a base class to make things more neat.
 * 
 * This will be treated as an Abstract Class and never have
 * a concrete instance of it unless its a derivative.
 * 
 * @author Diego Cordova
 */

class Entity
{
public:
	enum class Type
	{
		Player, Enemy, Bullet
	};

protected:
	std::vector <sf::Texture*> textures;
	sf::Sprite sprite;
	Type objectType;
	float scale,
		baseDamage, velocity, sway;

public:
	///Maybe add more constructors
	virtual ~Entity() {};	///Delete textures on derivative

	//Getters
	virtual bool isActive() const = 0;

	float getBaseDamage() const;
	float getLargestSide() const;
	sf::Vector2<float> getPos() const;
	sf::FloatRect bounds() const;
	float getVelocity() const;
	float getSway() const;

	//Setters
	void setBaseDamage(float dmg);
	void setVelocity(float v);
	void setSway(float s);
	void setPos(sf::Vector2<float> pos);

	//Draw Components
	virtual void update(sf::RenderTarget& target) = 0;
	virtual void render(sf::RenderTarget& target) = 0;
};

