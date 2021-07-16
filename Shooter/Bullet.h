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

private:
	//-Attributes
	sf::Vector2<float> target;
	Loop loop;
	bool active, isEnemy;

	void initVariables();
	void initSprite();

public:
	Bullet(float x, float y, Type t = Type::Player);
	Bullet(float x, float y, short shotSpeed, Loop l, Type t = Type::Player);
	~Bullet();

	//-Getter
	bool isActive() const override;
	sf::Vector2<float> getTarget() const;

	//-Setters
	void activate();
	void deactivate();
	void setTarget(float x, float y);
	void setVelocity(float x, float y);

	//-Functions
	void setColor(float r = -1, float g = -1, float b = -1, float alpha = 0);
	void setAngle(float destX, float destY);
	void setSway(float s);
	void setLoop(Loop l);

	//-Draw components
	void update(sf::RenderTarget& target) override;
	void render(sf::RenderTarget& target) override;
};

