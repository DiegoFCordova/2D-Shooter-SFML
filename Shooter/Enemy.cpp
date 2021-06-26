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
	maxHP = (int) hp;
	maxBullets = 10;
	state = State::Alive;
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

	sprite.setOrigin(sprite.getOrigin().x + (sprite.getLocalBounds().width / 2), sprite.getOrigin().y + (sprite.getLocalBounds().height / 2));

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
bool Enemy::isAlive() const
{
	return (state == State::Alive);
}

/*
 * Method to save a few accesses for death
 * animation.
 *
 * @return width/height of sprite (Global)
 */
float Enemy::getLargestSide()
{
	return (sprite.getGlobalBounds().width > sprite.getGlobalBounds().height) ? sprite.getGlobalBounds().width : sprite.getGlobalBounds().height;
}

/*
 * Calculates how much damage the bullet makes.
 *
 * @param k: Index of Bullet in bullets vector.
 */
float Enemy::damageDealt(int k)
{
	return bullets[k]->atk();		///Add damage multiplier later
}

/*
 * Used to ultimately delete mob after all bullets are gone.
 * @return Number of bullets left in screen.
 */
int Enemy::bulletsInScreen()
{
	return bullets.size();
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
	bullets.emplace_back(new Bullet(sprite.getPosition().x, sprite.getPosition().y + sprite.getGlobalBounds().height/2, 0));

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
			angle = (float) atan((diffX > diffY) ? diffX / diffY : diffY / diffX) * 180.f / 3.14159265f;

			b->setVelocity(vX, -vY);
			b->setEnemyBullet(angle, diffX, diffY, rightSide);
		}
	}
	//			sprite.move(sway, -velocity);
	//if (canAttack())
//		bullets.emplace_back(new Bullet(sprite.getPosition().x + (sprite.getGlobalBounds().width / 2), sprite.getPosition().y, sway));
}

/*
 * Used to calculate damage against enemy.
 */
void Enemy::takeDamage(float dmg)
{
	hp -= dmg;

	if (hp < maxHP / 3)
		sprite.setColor(sf::Color::Red);

	if (hp <= 0)
	{
		if (bullets.size() == 0)
			state = State::Death;
		else
			state = State::WaitingDisposal;
	}
}

/*
 * Called from Game when enemy has no
 * bullets left in the field.
 */
void Enemy::setWaitForDisposal()
{
	sprite.setColor(sf::Color(0, 0, 0, 0));
}

/*
 * ///If fall is true, enemy will start moving on its velocity.
 * 
 * @param target: Used to get window's bounds.
 */
void Enemy::update(sf::RenderTarget& target)
{
	///movement of enemy goes here
	/*if (sprite.getPosition().x > target.getSize().x)
		sprite.setPosition(0, sprite.getPosition().y);
	else
		sprite.move(0, 0);*/

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
