#pragma once

#include "Mob.h"

/*
 * Player class for user.
 * 
 * @author Diego Cordova
 */

class Player : public Mob
{
public:
	enum class Status
	{
		Alive, Dead, Invul
	};

private:
	//bool alive;
	Status status;
	float invulDur, invulCounter;
	short lives;

	void initVariables();  
	void initSprite();

public:
	//-Constructors
	Player();
	Player(float x, float y, float scaling);
	virtual ~Player();

	bool isActive() const override;
	short getLives() const;
	float getShotSpeed() const;
	Player::Status getStatus() const;

	//-Setters
	void setShotSpeed(float s);
	void setLives(short l);

	//-Functions
	void revive();
	void resetMob() override;
	void takeDamage(float dmg) override;
	void attackTo(float dstX, float dstY) override;

	//-Draw Components
	void update(sf::RenderTarget& target) override;
	void updateInput();
	void render(sf::RenderTarget& target) override;
};
