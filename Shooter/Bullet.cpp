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
	objectType = Type::Bullet;
	loop = Loop::None;
	active = true;
	isEnemy = false;
	target.x = -1;
	target.y = -1;
	scale = 1.5; ///2
	baseDamage = 3;
	velocity = 8;
	sway = 0;
	loopLimit.x = 4;
	loopLimit.y = 1;
	loopCounter.x = 0;
	loopCounter.y = 0;
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
}

/*
 * Calls both init for sprite and basic parameters.
 * Creates bullet at a specific place.
 *
 * @param x: x coordinate.
 * @param y: y coordinate.
 * @param t: Enumerator to identify type of object (Default = Player).
 */
Bullet::Bullet(float x, float y, Type t)
{
	initVariables();
	initSprite();
	sprite.setPosition(x, y - sprite.getGlobalBounds().height / 2);

	sprite.setOrigin(sprite.getOrigin().x + (sprite.getLocalBounds().width / 2), sprite.getOrigin().y + (sprite.getLocalBounds().height / 2));

	if (t == Type::Enemy)
	{
		isEnemy = true;
		setColor();
	}
}

Bullet::Bullet(float x, float y, short shotSpeed, Loop l, Type t)
{
	initVariables();
	initSprite();
	sprite.setPosition(x, y - sprite.getGlobalBounds().height / 2);

	sprite.setOrigin(sprite.getOrigin().x + (sprite.getLocalBounds().width / 2), sprite.getOrigin().y + (sprite.getLocalBounds().height / 2));

	velocity = shotSpeed;
	loop = l;

	if (t == Type::Enemy)
	{
		isEnemy = true;
		setColor();
	}
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
 * @return Limit of how many times can a bullet loop.
 */
sf::Vector2<short> Bullet::getLoopLimit() const
{
	return loopLimit;
}

/*
 * @return Number of loops a bulled have made so far.
 */
sf::Vector2<short> Bullet::getLoopCounter() const
{
	return loopCounter;
}

/*
 * @return true if active is true.
 */
bool Bullet::isActive() const
{
	return active;
}

/*
 * @return target with x and y float values.
 */
sf::Vector2<float> Bullet::getTarget() const
{
	return target;
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
 * @param n: new value for LoopLimit.
 */
void Bullet::setLoopLimit(sf::Vector2<short> n)
{
	loopLimit = n;
}

/*
 * @param n: new value for LoopCounter.
 */
void Bullet::setLoopCounter(sf::Vector2<short> n)
{
	loopCounter = n;
}

/*
 * Sets bullet sprite to certain color.
 * If not arguments present, sets it red for enemy.
 * 
 * @param r: Red.
 * @param g: Green.
 * @param b: Blue.
 * @param a: Alpha.
 */
void Bullet::setColor(float r, float g, float b, float a)
{
	if (isEnemy && r == -1)
		sprite.setColor(sf::Color(255, 50, 54, 150));

	else
		sprite.setColor(sf::Color(r, g, b, a));	//Nice color: 0, 100, 154, 100
}

/*
 * Rotates sprite towards the direction is being fired at.
 * 
 * @param destX: x coor of target/destination.
 * @param destY: y coor of target/destination.
 */
void Bullet::setAngle(float destX, float destY)
{
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
/* Sets sway for Bullet */
void Bullet::setSway(float s)
{
	sway = s;
}

/*
 * Sets looping ability to bullet.
 * 
 * @param l: new state for Loop.
 */
void Bullet::setLoop(Loop l)
{
	loop = l;
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
			if (loopCounter.y != -1)
				loopCounter.y++;

			if ((loop == Loop::Vertical || loop == Loop::All) && loopCounter.y <= loopLimit.y)
				sprite.setPosition(x, targetHeight + height / 2);
			else
				active = false;
		}

		else if (y > targetHeight + height / 2)
		{
			if (loopCounter.y != -1)
				loopCounter.y++;

			if ((loop == Loop::Vertical || loop == Loop::All) && loopCounter.y <= loopLimit.y)
				sprite.setPosition(x, 0 - height / 2);
			else
				active = false;
		}
		else if (x < 0 - width/2)
		{
			if (loopCounter.x != -1)
				loopCounter.x++;

			if ((loop == Loop::Horizontal || loop == Loop::All) && loopCounter.x <= loopLimit.x)
				sprite.setPosition(targetWidth + width/2, y);
			else
				active = false;
		}
		else if (x > targetWidth + width / 2)
		{
			if (loopCounter.x != -1)
				loopCounter.x++;

			if ((loop == Loop::Horizontal || loop == Loop::All) && loopCounter.x <= loopLimit.x)
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
