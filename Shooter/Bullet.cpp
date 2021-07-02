#include "Bullet.h"

/*
 * Initialize basic variables to default bullet value:
 * ----type: Decides what texture bullet will have.---
 * -baseDamage: How much damage it will deal.
 * -velocity: How fast it moves vertically.
 * -sway: How fast it moves horizontally.
 * -scale: How big it will be ///"Planned to increase when damage is bigger".
 * -active: true if bullet flying.
 */
void Bullet::initVariables()
{
	loop = Loop::None;
	color = From::Player;
	active = true;
	target.x = -1;
	target.y = -1;
	scale = 1.5; ///2
	baseDamage = 3;
	velocity = 3;
	sway = 0;
}

/*
 * texture loads certain image and sets it for sprite.
 * Checks if texture loading failed, if it does,
 * print message and return.
 */
void Bullet::initSprite()
{
	textures.emplace_back(new sf::Texture());

	if (!textures[0]->loadFromFile("Sprites/Bullet.png"))
	{
		std::cout << "Error loading Bullet's Sprite.\n";
		return;
	}

	sprite.setTexture(*textures[0]);
	sprite.scale(scale, scale);
	sprite.setPosition(200, 200);
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

/*
 * Destructor for pointers if needed.
 */
Bullet::~Bullet()
{
	for (auto* t : textures)
		delete t;
}

/*
 * @return true if active is true.
 */
bool Bullet::isActive() const
{
	return active;
}

/*
 * actives a bullet (Makes active = true).
 */
void Bullet::activate()
{
	active = true;
}

/*
 * Deactivates bullet, mainly used for when its offscreen.
 */
void Bullet::deactivate()
{
	active = false;
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
 * 
 * ///Change name to setAngleAndColor
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


void Bullet::setAngle(float destX, float destY)
{
	///switch for what color is chosen, maybe set it from constructor.

	float distance = 0, vX = 0, vY = 0, 
		x = sprite.getPosition().x, y = sprite.getPosition().y,
		diffX = 0, diffY = 0, angle = 0;
	bool rightSide = (destX > x) ? false : true,
		upperSide = (destY < y) ? false : true;
	setTarget(destX, destY);

	distance = sqrt(((destX - x) * (destX - x)) + ((destY - y) * (destY - y)));
	vX = (destX - x) / distance * velocity;
	vY = (destY - y) / distance * velocity;
	diffX = abs(destX - x);
	diffY = abs(destY - y);
	angle = (float)atan((diffX > diffY) ? diffX / diffY : diffY / diffX) * 180.f / 3.14159265f;

	setVelocity(vX, -vY);

	if (upperSide)
	{
		if (rightSide && diffX > diffY)
			angle += 180;
		else if (rightSide && diffY >= diffX)
			angle = 270 - angle;
		else if (!rightSide && diffY > diffX)
			angle = 90 + angle;
		else if (!rightSide && diffX >= diffY)
			angle = -angle + 180;
	}

	else
	{
		if (rightSide && diffX > diffY)
			angle = -angle;
		else if (rightSide && diffY >= diffX)
			angle = angle - 90;
		else if (!rightSide && diffY > diffX)
			angle = 90 - angle;
	}

	sprite.setRotation(angle);
}

/*
 * If active is true, bullet will move depending on its velocity.
 * Variables were set to avoid multiple callings.
 * First 3 if blocks check if it goes out of bounds to loop or delete.
 * 
 * @param target: Used to get window's bounds.
 */
void Bullet::update(sf::RenderTarget& target)
{
	if (active)
	{
		float x = sprite.getPosition().x, y = sprite.getPosition().y,
			width = sprite.getLocalBounds().width * scale, height = sprite.getLocalBounds().height * scale;
		int targetWidth = target.getSize().x, targetHeight = target.getSize().y;

		if (y < 0 - height/2)
		{
			if (loop == Loop::Vertical || loop == Loop::All)
				sprite.setPosition(x, targetHeight + height / 2);
			else
				active = false;
		}

		else if (y > targetHeight + height / 2)
		{
			if (loop == Loop::Vertical || loop == Loop::All)
				sprite.setPosition(x, 0 - height / 2);
			else
				active = false;
		}
		else if (x < 0 - width/2)
		{
			if (loop == Loop::Horizontal || loop == Loop::All)
				sprite.setPosition(targetWidth + width/2, y);
			else
				active = false;
		}
		else if (x > targetWidth + width / 2)
		{
			if (loop == Loop::Horizontal || loop == Loop::All)
				sprite.setPosition(0 - width/2, y);
			else
				active = false;
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
