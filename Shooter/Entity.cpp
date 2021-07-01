#include "Entity.h"

/*
 * Base constructor. Will never be used.
 */
Entity::Entity()
{
}
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

/*
 * @return target with x and y float values.
 */
sf::Vector2<float> Entity::getTarget() const
{
	return target;
}

/* @return sprite's global bounds. */
sf::FloatRect Entity::bounds() const
{
	return sprite.getGlobalBounds();
}
