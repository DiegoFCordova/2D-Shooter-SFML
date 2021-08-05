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
	Bullet::Loop continum;
	sf::Vector2<short> loopLimit;
	float hp, shotSpeed, cooldownCounter,
		frame, aniSpeed;
	short maxHP, maxBullets, shotRate;
	bool cooldown;

public:
	//Getter
	std::vector<Bullet*>& getBullets();
	Bullet::Loop getContinum() const;
	sf::Vector2<short> getLoopLimit() const;
	float getCurrentHP() const;
	short getMaxHP() const;
	int activeBullets() const;
	short getShotRate() const;

	//Functions
	float damageDealt(int k);
	bool canAttack();
	void setContinum(Bullet::Loop l);
	void setShotRate(short n);
	void setLoopLimit(sf::Vector2<short> n);
	void setMaxHP(float h);

	virtual void resetMob() = 0;
	virtual void takeDamage(float dmg) = 0;
	virtual void attackTo(float dstX, float dstY);
};

