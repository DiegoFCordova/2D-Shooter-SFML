#include "Enemy.h"

/*
 * Initialize basic variables to default for common enemy.
 */
void Enemy::initVariables()
{
	objectType = Type::Enemy;
	continum = Bullet::Loop::None;
	bullets.reserve(50);
	baseDamage = .8;
	maxHP = 10;
	hp = maxHP;
	frame = 0;
	aniSpeed = 25;
	cooldown = false;
	maxBullets = 100;
	shotRate = (int)(rand()%10) + 20;
	shotSpeed = 10;
	shotOffset = 100;
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
 * Sets Enemy fireRate and some more depending
 * on game difficulty.
 * 
 * @param x: x coordinate.
 * @param y: y coordinate.
 * @param diff: Current game difficulty.
 */
Enemy::Enemy(float x, float y, UI::Difficulty diff)
{
	initVariables();
	initSprite();

	switch (diff)
	{
	case UI::Difficulty::Easy:
		shotRate += 120 + (int)(rand() % 50);
		maxHP = 5;
		hp = maxHP;
		shotSpeed = 5;
		shotOffset = 10;
		sprite.setColor(sf::Color::Yellow);
		break;
	//case UI::Difficulty::Normal:
	//	sprite.setColor(sf::Color::Yellow);
	//	break;
	case UI::Difficulty::Merciless:
		shotRate -= 10;
		maxHP = 15;
		hp = maxHP;
		shotSpeed = 12;
		shotOffset = 200;
		sprite.setColor(sf::Color(150, 0, 255));

		break;
	default:
		break;
	}

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
 * Reset Mob values and delete all bullets.
 */
void Enemy::resetMob()
{
	bullets.clear();

	continum = Bullet::Loop::None;
	bullets.reserve(50);
	maxHP = 10;
	hp = maxHP;
	frame = 0;
	aniSpeed = 25;
	cooldown = false;
	maxBullets = 100;
	shotRate = (int)(rand() % 10) + 20;		///The extra int might change due to difficulty.
	shotSpeed = 16;
	shotOffset = 100;
	state = State::Alive;
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
		int offsetX = (int)(rand() % shotOffset) - (shotOffset / 2),
			offsetY = (int)(rand() % shotOffset) - (shotOffset / 2);

		bullets.emplace_back(new Bullet(sprite.getPosition().x,
			sprite.getPosition().y + sprite.getGlobalBounds().height / 2, shotSpeed, continum, objectType));
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
