#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

/*
 * Base enemy class for mobs.
 * 
 * @author MellamoSteve
 */

class Enemy
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
	bool fall;

	sf::Texture texture;
	sf::Sprite sprite;

	void initVariables();
	void initSprite();

public:
	Enemy();
	Enemy(float x, float y);
	~Enemy();

	//-Getters
	sf::FloatRect bounds() const;

	//-Functions


	//-Draw Components
	void update(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);


};

