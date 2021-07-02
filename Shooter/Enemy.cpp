#include "Enemy.h"

/*
 * Initialize basic variables to default for common enemy.
 */
void Enemy::initVariables()
{
	bullets.reserve(10);
	hp = 100;
	target.x = -1;
	target.y = -1;
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
	textures.emplace_back(new sf::Texture());
	if (!textures[0]->loadFromFile("Sprites/Enemy.png"))
		std::cout << "Error loading Player Sprite.\n";
	sprite.setTexture(*textures[0]);

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
	for (auto* k : bullets)
		delete k;

	for (auto* t : textures)
		delete t;
}

/*
 * @returns alive boolean in Enemy.
 */
bool Enemy::isActive() const
{
	return (state == State::Alive);
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
