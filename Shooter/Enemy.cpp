#include "Enemy.h"

/*
 * Initialize basic variables to default for common enemy.
 */
void Enemy::initVariables()
{
	bullets.reserve(10);
	hp = 10;
	xTarget = -1;
	yTarget = -1;
	maxHP = hp;
	maxBullets = 10;
	alive = true;
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

	sprite.scale(2, 2);
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
 * Get corner position of sprite.
 */
sf::Vector2<float> Enemy::getPos() const
{
	return sprite.getPosition();
}

/*
 * Get center of sprite.
 * ///Might need to take in consideration scaling.
 */
sf::Vector2<float> Enemy::getCenter() const
{
	sf::Vector2<float> center;
	center.x = sprite.getPosition().x + (sprite.getGlobalBounds().width / 2);
	center.y = sprite.getPosition().y;
	return center;
}

/*
 * @return sprite's global bounds.
 */
sf::FloatRect Enemy::bounds() const
{
	return sprite.getGlobalBounds();
}

/*
 * @return Player's bullets (Would it be better to make bullets public?)
 */
std::vector<Bullet*>& Enemy::getBullets()
{
	return bullets;
}

/*
 * @returns alive boolean in Enemy.
 */
bool Enemy::isAlive()
{
	return alive;
}

/*
 * Used to calculate damage against enemy.
 */
void Enemy::takeDamage(float dmg)
{
	hp -= dmg;

	if (hp < maxHP/3)
		sprite.setColor(sf::Color::Red);

	if (hp <= 0)
		alive = false;
}
/*
 * Shoots bullet to specific direction.
 * 
 * @param x: X coor of the target.
 * @param y: Y coor of the target.
 */
void Enemy::attack(float x, float y)
{
	//if maxBullets
	bullets.emplace_back(new Bullet(sprite.getPosition().x + (sprite.getGlobalBounds().width / 2), sprite.getPosition().y + sprite.getGlobalBounds().height/2*3, 0));

	for (auto* b : bullets)
	{
		if (b->getTarget().x == -1 && b->getTarget().y == -1)
		{
			float distance = 0, vX = 0, vY = 0, bulletVel = 3;
			float diffX = 0, diffY = 0, angle = 0;
			bool rightSide = (x > b->getPos().x) ? false : true;
			b->setTarget(x, y);

			distance = sqrt( ((x - b->getPos().x) * (x - b->getPos().x)) + ((y - b->getPos().y) * (y - b->getPos().y)));
			vX = (x - b->getPos().x) / distance * bulletVel;
			vY = (y - b->getPos().y) / distance * bulletVel;
			diffX = (x > b->getPos().x) ? x - b->getPos().x : b->getPos().x - x;
			diffY = (y > b->getPos().y) ? y - b->getPos().y : b->getPos().y - y;
			angle = atan((diffX > diffY) ? diffX / diffY : diffY / diffX) * 180.0 / 3.14159265;

			b->setVelocity(vX, -vY);
			b->setEnemyBullet(angle, diffX, diffY, rightSide);
		}
	}
	//			sprite.move(sway, -velocity);
	//if (canAttack())
//		bullets.emplace_back(new Bullet(sprite.getPosition().x + (sprite.getGlobalBounds().width / 2), sprite.getPosition().y, sway));
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
		sprite.move(0, 0);

	for (auto* k : bullets)
		k->update(target);
}

/*
 * Draws the sprite into target.
 */
void Enemy::render(sf::RenderTarget& target)
{
	target.draw(sprite);

	for (auto* k : bullets)
		k->render(target);
}
