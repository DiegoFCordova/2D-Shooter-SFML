#include "Bullet.h"

/*
 * Initialize basic variables to default bullet value:
 * ----type: Decides what texture bullet will have.---
 * -baseDamage: How much damage it will deal.
 * -velocity: How fast it moves vertically.
 * -sway: How fast it moves horizontally.
 * -scaling: How big it will be ///"Planned to increase when damage is bigger".
 * -fire: true if bullet flying.
 */
void Bullet::initVariables()
{
	loop = Loop::None;
	target.x = -1;
	target.y = -1;
	baseDamage = 3;
	velocity = 3;
	sway = 0;
	scaling = .5; ///2
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
	sprite.setPosition(x, y - sprite.getGlobalBounds().height/2);
	this->sway = sway;

	sprite.setOrigin(sprite.getOrigin().x + (sprite.getLocalBounds().width/2), sprite.getOrigin().y + (sprite.getLocalBounds().height/2));
}

///*
// * Same as above, but this one specifies type of bullet.
// */
//Bullet::Bullet(float x, float y, Type type)
//{
//	this->type = type;
//	initVariables();
//	initSprite();
//	sprite.setPosition(x, y);
//}
//
///*
// * Same as above, but this one also specifies scaling of sprite.
// */
//Bullet::Bullet(float x, float y, float scale, Type type)
//{
//	scaling = scale;
//	this->type = type;
//	initVariables();
//	initSprite();
//	sprite.setPosition(x, y);
//}

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

float Bullet::atk() const
{
	return baseDamage;
}

/*
 * @return sprite's global bounds.
 */
sf::FloatRect Bullet::bounds() const
{
	return sprite.getGlobalBounds();
}

/*
 * @return target with x and y float values.
 */
sf::Vector2<float> Bullet::getTarget() const
{
	return target;
}

/*
 * @return Position of the Bullet.
 */
sf::Vector2<float> Bullet::getPos() const
{
	return sprite.getPosition();
}

/*
 * Fires a bullet (Makes fire = true).
 */
void Bullet::activate()
{
	fire = true;
}

/*
 * Deactivates bullet, mainly used for when its offscreen.
 */
void Bullet::deactivate()
{
	fire = false;
}

/*
 * 
 */
void Bullet::setTarget(float x, float y)
{
	target.x = x;
	target.y = y;
}

/*
 * Change the velocity of the bullet.
 * 
 * @param x: for the X coordinate.
 * @param y: for the Y coordinate.
 */
void Bullet::setVelocity(float x, float y)
{
	velocity = y;
	sway = x;
}

/*
 * Changes color to more redish.
 * Rotates the sprite so that its aiming its target
 * (Only when target its below. Might do for above later).
 * 
 * @param angle: Offset of the final angle calculation.
 * @param x: Length of triangle.
 * @param y: Height of triangle.
 * @param side: important part for final angle calculation.
 */
void Bullet::setEnemyBullet(float angle, float x, float y, bool side)
{
	sprite.setColor(sf::Color::Red);

	if (side && x > y)
		angle += 180;
	else if (side && y >= x)
		angle = 270 - angle;
	else if (!side && y > x)
		angle = 90 + angle;
	else if (!side && x >= y)
		angle = -angle + 180;

	sprite.setRotation(angle);
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

		if (y < 0 - height/2)
		{
			if (loop == Loop::Vertical || loop == Loop::All)
				sprite.setPosition(x, targetHeight + height / 2);
			else
				fire = false;
		}

		else if (y > targetHeight + height / 2)
		{
			if (loop == Loop::Vertical || loop == Loop::All)
				sprite.setPosition(x, 0 - height / 2);
			else
				fire = false;
		}
		else if (x < 0 - width/2)
		{
			if (loop == Loop::Horizontal || loop == Loop::All)
				sprite.setPosition(targetWidth + width/2, y);
			else
				fire = false;
		}
		else if (x > targetWidth + width / 2)
		{
			if (loop == Loop::Horizontal || loop == Loop::All)
				sprite.setPosition(0 - width/2, y);
			else
				fire = false;
		}
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
