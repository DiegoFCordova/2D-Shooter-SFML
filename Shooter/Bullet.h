#pragma once

#include "Entity.h"

/*
 * Base bullet class for game's projectiles.
 * ///Might make it into a base class and variations into sub
 * 
 * @author Diego Cordova
 */

class Bullet : public Entity
{
	///Make Enum for Variation sprite. Prototype
public:
	enum class Loop
	{
		None, Horizontal, Vertical, All
	};

	///Type of Bullet. Used to decide color.
	enum class From
	{
		Player, Enemy
	};

private:
	//-Attributes
	Loop loop;
	From color;
	bool active;

public:

	void initVariables();
	void initSprite();

	///Delete Following 1 or add scaling parameter
	Bullet(float x, float y, float sway);
	///Think for later: Modifiers, multipliers, etc
	//Bullet(float x, float y, Type type);
	//Bullet(float x, float y, float scale, Type type);
	~Bullet();

	//-Getter
	bool isActive() const override;

	//-Setters
	void activate();
	void deactivate();
	void setTarget(float x, float y);
	void setVelocity(float x, float y);

	//-Functions
	void setEnemyBullet(float angle, float x, float y, bool side);	//Maybe add an enum for this
	void setAngle(float destX, float destY);

	//-Draw components
	void update(sf::RenderTarget& target) override;
	void render(sf::RenderTarget& target) override;
};

