#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class Entity
{
private:
	int velocity;

	sf::Texture texture;
	sf::Sprite sprite;

	virtual void initVariables() = 0;
	virtual void initSprite() = 0;

public:


	virtual void render(sf::RenderTarget& target) = 0;
	virtual void update() = 0;
};