#include "Player.h"

/*
 * Initialize basic variables to default for player.
 */
void Player::initVariables()
{
	objectType = Type::Player;
	bullets.reserve(50);
	shotRate = 1;
	cooldownCounter = 0;
	cooldown = false;
	frame = 0;
	aniSpeed = 25;
	alive = true;
	velocity = 15;
	sway = 0;
	maxHP = 10;
	hp = maxHP;
	maxBullets = 1000;
	scale = 1.5;
}

/*
 * texture loads certain image and sets it for sprite.
 * Checks if texture loading failed, if it does,
 * print message and return.
 */
void Player::initSprite()
{
	textures.emplace_back(new sf::Texture());
	textures.emplace_back(new sf::Texture());

	if (!textures[0]->loadFromFile("Sprites/Ship0.png"))
		std::cout << "Error loading Player Sprite 1.\n";
	if (!textures[1]->loadFromFile("Sprites/Ship1.png"))
		std::cout << "Error loading Player Sprite 2.\n";
	sprite.setTexture(*textures[0]);
	sprite.scale(scale, scale);
}

/*
 * Default constructor, will never be used
 * but defined just in case.
 */
Player::Player()
{
	initVariables();
	initSprite();
}

/*
 * Calls both init for sprite and basic parameters.
 * Creates bullet at a specific place.
 */
Player::Player(float x, float y, float scale)
{
	initVariables();
	initSprite();

	sprite.setOrigin(sprite.getPosition().x + (sprite.getLocalBounds().width / 2), sprite.getPosition().y + (sprite.getLocalBounds().height / 2));
	sprite.setPosition(x, y);
}

/* Destructor for pointers. */
Player::~Player()
{
	for (auto *k : bullets)
		delete k;

	for (auto* t : textures)
		delete t;
}

/* @return true if player is alive. */
bool Player::isActive() const
{
	return alive;
}

/// Delete later
float Player::getSway()
{
	return sway;
}

/*
 * Called when player takes damage.
 * If hp falls below 0, bool alive = false.
 *
 * @param dmg: damage taken.
 *
 * ///Maybe add an afterdamage invul frames
 */
void Player::takeDamage(float dmg)
{
	hp -= dmg;

	if (hp < 0)
	{
		alive = false;
		//sprite.setColor(sf::Color(0, 0, 0, 0));
	}
}

/*
 * Shoots bullet to specific direction.
 * Override: Places bullet in specific location
 * for current Object.
 *
 * @param x: X coor of the target.
 * @param y: Y coor of the target.
 */
void Player::attackTo(float dstX, float dstY)
{
	bullets.emplace_back(new Bullet(sprite.getPosition().x, sprite.getPosition().y - sprite.getGlobalBounds().height / 2));
	Mob::attackTo(dstX, dstY);
}

/*
 * Update animations.
 * Also loops player if he goes outside screen.
 */
void Player::update(sf::RenderTarget& target)
{
	float x = sprite.getPosition().x, y = sprite.getPosition().y,
		width = sprite.getGlobalBounds().width, height = sprite.getGlobalBounds().height;
	int targetWidth = target.getSize().x, targetHeight = target.getSize().y;

	frame++;
	if (frame == aniSpeed)
	{
		sprite.setTexture(*textures[1]);
		frame = -aniSpeed;
	}
	else if (frame == 0)
		sprite.setTexture(*textures[0]);

	if(cooldown && cooldownCounter < shotRate + 3)
		cooldownCounter++;
	updateInput();

	for (auto* k : bullets)
		k->update(target);

	if (y < 0 - height / 2)
		sprite.setPosition(x, targetHeight + height / 2);
	else if (y > targetHeight + height / 2)
		sprite.setPosition(x, 0 - height / 2);
	else if (x < 0 - width / 2)
		sprite.setPosition(targetWidth + width / 2, y);
	else if (x > targetWidth + width / 2)
		sprite.setPosition(0 - width / 2, y);
}

/*
 * "Translate" keyboard inputs into actions.
 */
void Player::updateInput()
{
	bool moving = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		sprite.move(-velocity, 0.f);

		if(sway > -velocity / 4) 
			sway -= .2;
		//sway = -velocity / 4;
		moving = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		sprite.move(velocity, 0.f);

		if (sway < velocity / 4)
			sway += .2;
		//sway = velocity / 4;
		moving = true;
	}

	if (!moving)
	{
		if (sway > -.3 && sway < .3)
			sway = 0;
		else
			sway += (sway < 0) ? .5 : -.5;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		sprite.move(0.f, -velocity);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		sprite.move(0.f, velocity);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		if (canAttack())
		{
			bullets.emplace_back(new Bullet(sprite.getPosition().x, sprite.getPosition().y - sprite.getGlobalBounds().height / 2));
			bullets[bullets.size() - 1]->setSway(sway);
		}
}

/*
 * Draws sprite and its objects into target.
 * 
 * @param target: Destination of drawing.
 */
void Player::render(sf::RenderTarget& target)
{
	target.draw(sprite);

	for (auto* k : bullets)
		k->render(target);
}
