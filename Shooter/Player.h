#pragma once

#include "Mob.h"

/*
 * Player class for user.
 * 
 * @author Diego Cordova
 */

class Player : public Mob
{
private:
	bool alive;

	void initVariables();  
	void initSprite();

public:
	//-Constructors
	Player();
	Player(float x, float y, float scaling);
	virtual ~Player();

	bool isActive() const override;

	///Delete later
	float getSway();

	//-Setters

	//-Functions
	void takeDamage(float dmg) override;

	//-Draw Components
	void update(sf::RenderTarget& target) override;
	void updateInput();
	void render(sf::RenderTarget& target) override;
};
