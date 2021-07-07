#include "Enemy.h"

/*
 * Initialize basic variables to default for common enemy.
 */
void Enemy::initVariables()
{
	objectType = Type::Enemy;
	bullets.reserve(10);
	hp = 10;
	frame = 0;
	aniSpeed = 25;
	maxHP = (int) hp;
	cooldown = false;
	maxBullets = 100;
	shotRate = (int)(rand()%10) + 20;
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
	textures.emplace_back(new sf::Texture());
	if (!textures[0]->loadFromFile("Sprites/Enemy0.png"))
		std::cout << "Error loading Player Sprite 1.\n";
	if (!textures[1]->loadFromFile("Sprites/Enemy1.png"))
		std::cout << "Error loading Player Sprite 2.\n";
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
 * Shoots bullet to specific direction.
 * Override: Places bullet in specific location
 * for current Object.
 *
 * @param x: X coor of the target.
 * @param y: Y coor of the target.
 */
void Enemy::attackTo(float dstX, float dstY)
{
	if (canAttack())
	{
		int offsetX = (int)(rand() % 100) - 50,
			offsetY = (int)(rand() % 100) - 50;
		bullets.emplace_back(new Bullet(sprite.getPosition().x, sprite.getPosition().y + sprite.getGlobalBounds().height / 2, objectType));
		Mob::attackTo(dstX + offsetX, dstY + offsetY);
	}
}

/*
 * ///If fall is true, enemy will start moving on its velocity.
 * 
 * @param target: Used to get window's bounds.
 */
void Enemy::update(sf::RenderTarget& target)
{
	frame++;
	if (frame == aniSpeed)
	{
		sprite.setTexture(*textures[1]);
		frame = -aniSpeed;
	}
	else if (frame == 0)
		sprite.setTexture(*textures[0]);

	if (cooldown && cooldownCounter < shotRate + 3)
		cooldownCounter++;

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
