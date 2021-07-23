#include "Entity.h"

/*
 * @return get baseDamage of object.
 */
float Entity::getBaseDamage() const
{
	return baseDamage;
}

/*
 * Method to save a few accesses for death
 * animation.
 *
 * @return width/height of sprite (Global)
 */
float Entity::getLargestSide() const
{
	return (sprite.getGlobalBounds().width > sprite.getGlobalBounds().height) ? sprite.getGlobalBounds().width : sprite.getGlobalBounds().height;
}

/*
 * @return Center position of Entity.
 */
sf::Vector2<float> Entity::getPos() const
{
	return sprite.getPosition();
}

/* @return sprite's global bounds. */
sf::FloatRect Entity::bounds() const
{
	return sprite.getGlobalBounds();
}

/*
 * @return Entity velocity.
 */
float Entity::getVelocity() const
{
	return velocity;
}

/*
 * @return sway of Entity
 */
float Entity::getSway() const
{
	return sway;
}

/*
 * Sets velocity to given param.
 * 
 * @param v: new Velocity.
 */
void Entity::setVelocity(float v)
{
	velocity = v;
}

/*
 * Set sway to given parameter.
 * 
 * @param s: new value for sway.
 */
void Entity::setSway(float s)
{
	sway = s;
}

/*
 * Sets new position for sprite.
 * 
 * @param pos: x and y coordinate.
 */
void Entity::setPos(sf::Vector2<float> pos)
{
	sprite.setPosition(pos.x, pos.y);
}
