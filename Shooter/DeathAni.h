#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <iostream>
#include "DT.h"

/*
 * Simple Death Animation class for when an enemy
 * is defeated.
 * 
 * @author Diego Cordova
 */

class DeathAni
{
public:
	enum class Type
	{
		Enemy, Bullet, Player
	};

private:
	std::vector <sf::Texture*> textures;
	sf::Sprite sprite;
	Type type;
	float scale, frames, aniSpeed;
	short currentFrame;
	bool done;

	void loadTextures(int n, const std::stringstream& str);

public:
	//-Constructors and Destructors
	DeathAni();
	DeathAni(float x, float y, float side, Type t = Type::Enemy);
	~DeathAni();

	//-Getters
	bool isOver() const;

	//-Functions
	void init();
	void setColor(float r = -1, float g = -1, float b = -1, float alpha = 0);
	float getFitScale(float side) const;

	void update(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);
};

