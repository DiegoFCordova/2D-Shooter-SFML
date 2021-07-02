#pragma once

#include "Bullet.h"

/*
 * Another Class that is mostly an interface made 
 * to avoid repeating that will have Player and Enemy as nodes.
 * 
 * @author Diego Cordova
 */

class Mob : public Entity
{
protected:
	std::vector <Bullet*> bullets;
	float hp;
	short maxHP, maxBullets, shotRate, cooldownCounter,
		frame, aniSpeed;
	bool cooldown;

public:
	//Getter
	std::vector<Bullet*>& getBullets();
	float getCurrentHP() const;
	short getMaxHP() const;
	int activeBullets() const;

	//Functions
	float damageDealt(int k);
	bool canAttack();
	virtual void takeDamage(float dmg) = 0;
	virtual void attackTo(float dstX, float dstY);
};

