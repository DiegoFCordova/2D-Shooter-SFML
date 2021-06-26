#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

/*
 * Simple Death Animation class for when an enemy
 * is defeated.
 * 
 * @author Diego Cordova
 */

class DeathAni
{
private:
	sf::Texture tex1, tex2, tex3, tex4;
	sf::Sprite sprite;
	float scale;
	int frames;
	bool done;

public:
	//-Constructors and Destructors
	DeathAni();
	DeathAni(float x, float y, float side);
	~DeathAni();

	//-Getters
	bool isOver() const;

	//-Functions
	void init();
	float getFitScale(float side) const;

	void update(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);
};

