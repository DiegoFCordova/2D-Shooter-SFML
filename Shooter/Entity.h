#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

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
private:
	std::vector <sf::Texture*> textures;
	sf::Sprite sprite;
	sf::Vector2<float> target;
	float scale,
		baseDamage, velocity, sway;
	///For Mob: HP, MaxHP

public:
	///Maybe add more constructors
	Entity();
	virtual ~Entity() {};	///Delete textures on derivative

	//Getters
	virtual bool isActive() const = 0;

	float getBaseDamage() const;
	float getLargestSide() const;
	sf::Vector2<float> getPos() const;
	sf::Vector2<float> getTarget() const;
	sf::FloatRect bounds() const;
	///For Mob: getBullets();, dmg dealt, Bullets in Screen(Bullets.size)
		///Atack, takedmg

	//Draw Components
	virtual void update(sf::RenderTarget& target) = 0;
	virtual void render(sf::RenderTarget& target) = 0;
};

