#pragma once
/*
 * Abstract class for enemy mobs
 */

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class Mob
{
	//Make Enum for Variation sprite
public:
	enum Type
	{
		Common, Strong, Fast
	};

private:
	///Mostly for initial, don't forget there is already x and y in Shapes
	int hp;

	sf::Texture texture;


	void initVariables();
	void initSprite();

public:
	sf::Sprite sprite;

	Mob();
	Mob(float x, float y);
	~Mob();

	void update(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);


};

