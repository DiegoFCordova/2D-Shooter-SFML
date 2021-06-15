#include "Bullet.h"

/*
 * Initialize basic variables to default bullet value:
 * -type: Decides what texture bullet will have.
 * -baseDamage: How much damage it will deal.
 * -velocity: How fast it moves vertically.
 * -sway: How fast it moves horizontally.
 * -scaling: How big it will be ///"Planned to increase when damage is bigger".
 * -fire: true if bullet flying.
 */
void Bullet::initVariables()
{
	type = Type::Common;
	baseDamage = 3;
	velocity = 3;
	sway = 0;
	scaling = 2;
	fire = true;
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
 *
 * @param x: x coordinate.
 * @param y: y coordinate.
 * @param sway: horizontal speed.
 */
Bullet::Bullet(float x, float y, float sway)
{
	initVariables();
	initSprite();
	sprite.setPosition(x - sprite.getGlobalBounds().width/2, y - sprite.getGlobalBounds().height);
	this->sway = sway;
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
bool Bullet::isActive() const
{
	return fire;
}

/*
 * @return sprite's global bounds.
 */
sf::FloatRect Bullet::bounds() const
{
	return sprite.getGlobalBounds();
}

/*
 * Fires a bullet (Makes fire = true).
 */
void Bullet::activate()
{
	fire = true;
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
	if (fire)
	{
		float x = sprite.getPosition().x, y = sprite.getPosition().y,
			width = sprite.getLocalBounds().width * scaling, height = sprite.getLocalBounds().height * scaling;
		int targetWidth = target.getSize().x, targetHeight = target.getSize().y;

		if (y < 0 - height)
			sprite.setPosition(x, targetHeight);
		else if (x < 0 - width)
			sprite.setPosition(targetWidth, y);
		else if (x > targetWidth)
			sprite.setPosition(0 - width, y);
		else
			sprite.move(sway, -velocity);
	}
}

/*
 * Draws the sprite into target.
 */
void Bullet::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
