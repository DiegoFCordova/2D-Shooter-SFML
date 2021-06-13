#include "Enemy.h"

/*
 * Initialize basic variables to default for common enemy.
 * 
 * @author MellamoSteve
 */
void Enemy::initVariables()
{
	hp = 100;
	fall = false;
}

/*
 * texture loads certain image and sets it for sprite.
 * Checks if texture loading failed, if it does,
 * print message and return.
 */
void Enemy::initSprite()
{
	if (!texture.loadFromFile("Sprites/Enemy.png"))
		std::cout << "Error loading Player Sprite.\n";
	sprite.setTexture(texture);

	sprite.scale(3, 3);
}

/*
 * Default constructor, most likely never used
 * but still defined just in case.
 */
Enemy::Enemy()
{
	initVariables();
	initSprite();
}

/*
 * Uses inits to set up sprite and basic parameters.
 * Sets position and originPosition to x and y.
 * 
 * @param x: x coordinate.
 * @param y: y coordinate.
 */
Enemy::Enemy(float x, float y)
{
	initVariables();
	initSprite();
	sprite.setPosition(x, y);
}

/*
 * Destructor for pointers if needed.
 */
Enemy::~Enemy()
{

}

/*
 * If fall is true, enemy will start moving on its velocity.
 * 
 * @param target: Used to get window's bounds.
 */
void Enemy::update(sf::RenderTarget& target)
{
	if (sprite.getPosition().x > target.getSize().x)
		sprite.setPosition(0, sprite.getPosition().y);
	else
		sprite.move(5, 0);
}

/*
 * Draws the sprite into target.
 */
void Enemy::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
