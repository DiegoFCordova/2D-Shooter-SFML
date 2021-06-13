#include "Bullet.h"

/*
 * Initialize basic variables to default bullet value.
 *
 * @author MellamoSteve
 */
void Bullet::initVariables()
{
	type = Type::Common;
	baseDamage = 3;
	velocity = 5;
	scaling = 3;
	fire = false;
}

/*
 * texture loads certain image and sets it for sprite.
 * Checks if texture loading failed, if it does,
 * print message and return.
 */
void Bullet::initSprite()
{
	if (!texture.loadFromFile("Sprites/Bullet.png"))
	{
		std::cout << "Error loading Bullet's Sprite.\n";
		return;
	}

	sprite.setTexture(texture);
	sprite.scale(scaling, scaling);
	sprite.setPosition(200, 200);
}

/*
 * Default constructor, will never be used
 * but defined just in case.
 */
Bullet::Bullet()
{
	initVariables();
	initSprite();
}

/*
 * Calls both init for sprite and basic parameters.
 * Creates bullet at a specific place.
 * ///Maybe make it so fire becomes true
 *
 * @param x: x coordinate.
 * @param y: y coordinate.
 */
Bullet::Bullet(float x, float y)
{
	initVariables();
	initSprite();
	sprite.setPosition(x, y);
}

/*
 * Same as above, but this one specifies type of bullet.
 */
Bullet::Bullet(float x, float y, Type type)
{
	this->type = type;
	initVariables();
	initSprite();
	sprite.setPosition(x, y);
}

/*
 * Same as above, but this one also specifies scaling of sprite.
 */
Bullet::Bullet(float x, float y, float scale, Type type)
{
	scaling = scale;
	this->type = type;
	initVariables();
	initSprite();
	sprite.setPosition(x, y);
}

/*
 * Destructor for pointers if needed.
 */
Bullet::~Bullet()
{

}

/*
 * @return true if fire is true.
 */
bool Bullet::isActive()
{
	return fire;
}

/*
 * If fire is true, bullet will move depending on its velocity.
 * Variables were set to avoid multiple callings.
 * First 3 if blocks check if it goes out of bounds to loop or delete.
 * 
 * @param target: Used to get window's bounds.
 */
void Bullet::update(sf::RenderTarget& target)
{
	float x = sprite.getPosition().x, y = sprite.getPosition().y,
		width = sprite.getLocalBounds().width * scaling, height = sprite.getLocalBounds().height * scaling;
	int targetWidth = target.getSize().x, targetHeight = target.getSize().y;

	if (y < 0 - height)
		sprite.setPosition(x, targetHeight);
	else if (x < 0 - width)
		sprite.setPosition(targetWidth, y);
	else if (x > targetWidth)
		sprite.setPosition(0, y);
	else
		sprite.move(0, -velocity);
}

/*
 * Draws the sprite into target.
 */
void Bullet::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
