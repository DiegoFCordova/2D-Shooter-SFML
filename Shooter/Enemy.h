#pragma once

#include "Mob.h"

/*
 * Base enemy class for mobs.
 * 
 * @author Diego Cordova
 */

class Enemy : public Mob
{
	//Make Enum for Variation sprite
public:
	/* Used for Enemy Type */
	//enum class MobType
	//{
	//	Common, Strong, Fast
	//};

	/*
 	 * States for easier enemy control:
	 *	  Alive: Enemy can move and shoot.
	 *	  WaitingDisposal: Only bullets move.
	 *	  Death: If no more bullets in the field and enemy is death.
	 */
	enum class State
	{
		Alive, WaitingDisposal, Death
	};

private:
	///Mostly for initial, don't forget there is already x and y in sprite
	State state;

	void initVariables();
	void initSprite();

public:
	Enemy();
	Enemy(float x, float y);
	~Enemy();

	//-Getters
	bool isActive() const override;

	//-Functions
	void takeDamage(float dmg) override;
	void setWaitForDisposal();
	void attackTo(float dstX, float dstY) override;

	//-Draw Components
	void update(sf::RenderTarget& target) override;
	void render(sf::RenderTarget& target) override;


};

