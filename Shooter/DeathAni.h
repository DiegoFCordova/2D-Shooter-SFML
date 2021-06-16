#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

/*
 * Simple Death Animation class for when an enemy
 * is defeated.
 */

class DeathAni
{
public:
	sf::Texture tex1, tex2, tex3, tex4;
	sf::Sprite sprite;
	int frames;
	bool done;

	DeathAni();
	DeathAni(float x, float y);

	bool isOver() const;

	void init();

	void update(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);
};

