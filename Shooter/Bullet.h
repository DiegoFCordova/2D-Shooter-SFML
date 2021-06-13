#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

/*
 * Base bullet class for game's projectiles.
 * ///Might make it into a base class and variations into sub
 * 
 * @author MellamoSteve
 */

class Bullet
{
	///Make Enum for Variation sprite. Prototype
public:
	enum class Type
	{
		Common, Strong, Fast
	};

private:
	/*		///Might delete later
	 * type: will decide the type of bullet
	 * baseDamage: Base damage of common bullet
	 * velocity: How fast it will go
	 * direction?: will decide if its moving to the side or not
	 * scaling: How big it will be (Maybe make it bigger the stronger it is
	 *								Keep in mind that this would involve basing hitbox in scale)
	 * fire: indicates if bullet is being fired or not
	 * Hit?: true if it hits, most likely used for points but I think I don't need it
	 */

	//-Attributes
	Type type;
	float baseDamage;
	float velocity;
	float scaling;
	bool fire;

	//-Basics
	sf::Texture texture;
	sf::Sprite sprite;

	void initVariables();
	void initSprite();

public:
	Bullet();
	///Delete Following 1 or add scaling parameter
	Bullet(float x, float y);
	///Think for later: Modifiers, multipliers, etc
	Bullet(float x, float y, Type type);
	Bullet(float x, float y, float scale, Type type);
	~Bullet();

	//-Getter
	bool isActive();

	//-Draw components
	void update(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);
};

