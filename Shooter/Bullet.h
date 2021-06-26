#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

/*
 * Base bullet class for game's projectiles.
 * ///Might make it into a base class and variations into sub
 * 
 * @author Diego Cordova
 */

class Bullet
{
	///Make Enum for Variation sprite. Prototype
public:
	enum class Loop
	{
		None, Horizontal, Vertical, All
	};

private:
	/*		///Might delete later
	 * target: For homing bullets. Might need a boolean for chasers.
	 * baseDamage: Base damage of common bullet
	 * velocity: How fast it will go
	 * sway: positive go to right, negative left
	 * scaling: How big it will be (Maybe make it bigger the stronger it is
	 *								Keep in mind that this would involve basing hitbox in scale)
	 * fire: indicates if bullet is being fired or not
	 * Hit?: true if it hits, most likely used for points but I think I don't need it
	 */

	//-Attributes
	Loop loop;
	sf::Vector2<float> target;
	float baseDamage, velocity, sway;
	float scaling;
	bool fire;

public:
	//-Basics
	sf::Texture texture;
	sf::Sprite sprite;

	void initVariables();
	void initSprite();

	Bullet();
	///Delete Following 1 or add scaling parameter
	Bullet(float x, float y, float sway);
	///Think for later: Modifiers, multipliers, etc
	//Bullet(float x, float y, Type type);
	//Bullet(float x, float y, float scale, Type type);
	~Bullet();

	//-Getter
	bool isActive() const;
	float atk() const;
	sf::FloatRect bounds() const;
	sf::Vector2<float> getTarget() const;
	sf::Vector2<float> getPos() const;

	//-Setters
	void activate();
	void deactivate();
	void setTarget(float x, float y);
	void setVelocity(float x, float y);

	//-Functions
	void setEnemyBullet(float angle, float x, float y, bool side);	//Maybe add an enum for this


	//-Draw components
	void update(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);
};

