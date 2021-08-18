#include "Player.h"

/*
 * Initialize basic variables to default for player.
 */
void Player::initVariables()
{
	objectType = Type::Player;
	continum = Bullet::Loop::None;
	bullets.reserve(50);
	baseDamage = 1;
	shotRate = 4;
	cooldownCounter = 0;
	cooldown = false;
	frame = 0;
	aniSpeed = 25;
	shieldDur = 250;
	shieldCounter = 0;
	status = Status::Alive;
	lives = 3;
	velocity = 15;
	shotSpeed = 8;
	loopLimit.x = -1;
	loopLimit.y = 4;
	sway = 0;
	maxHP = 20;
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

	hitbox.setFillColor(sf::Color(255, 255, 0, 0));
	hitbox.setSize(sf::Vector2f(20, 20));
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

	hitbox.setOrigin(hitbox.getPosition().x + (hitbox.getLocalBounds().width / 2), hitbox.getPosition().y + (hitbox.getLocalBounds().height / 2));
	hitbox.setPosition(x, y);
}

/* Destructor for pointers. */
Player::~Player()
{
	for (auto *k : bullets)
		delete k;

	for (auto* t : textures)
		delete t;
}

/* @return true if player is not dead. */
bool Player::isActive() const
{
	return (status != Status::Dead);
}

/* @return number of lives left for Player */
short Player::getLives() const
{
	return lives;
}

/*
 * @return shotSpeed of Player.
 */
float Player::getShotSpeed() const
{
	return shotSpeed;
}

/* @returns Current status of Player. */
Player::Status Player::getStatus() const
{
	return status;
}

/*
 * Makes hitbox Player's hitbox.
 */
sf::FloatRect Player::bounds() const
{
	return hitbox.getGlobalBounds();
}

/*
 * Sets shotSpeed to s.
 * 
 * @param s: new shotSpeed value.
 */
void Player::setShotSpeed(float s)
{
	shotSpeed = s;
}

/*
 * Change the number of lives left on Player.
 * 
 * @param l: New lives value.
 */
void Player::setLives(short l)
{
	lives = l;
}

/*
 * Method used to revive Player.
 */
void Player::revive()
{
	status = Status::Shield;
	hp = maxHP;
	shotRate = (shotRate == 1) ? 1 : shotRate / 2;
	shotSpeed *= 2;
	velocity *= 1.5;
	baseDamage *= 2;
}

/*
 * Reset Mob values and delete all bullets.
 */
void Player::resetMob()
{
	bullets.clear();
	sprite.setColor(sf::Color(255, 255, 255, 255));

	continum = Bullet::Loop::None;
	baseDamage = 1;
	shotRate = 4;
	cooldownCounter = 0;
	cooldown = false;
	frame = 0;
	aniSpeed = 25;
	shieldDur = 250;
	shieldCounter = 0;
	lives = 3;
	status = Status::Alive;
	velocity = 15;
	shotSpeed = 8;
	loopLimit.x = -1;
	loopLimit.y = 4;
	sway = 0;
	maxHP = 20;
	hp = maxHP;
	maxBullets = 1000;
	scale = 1.5;
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
		lives--;
		status = Status::Dead;
		sprite.setColor(sf::Color(0, 0, 0, 0));
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
 * Makes Player invulnerable
 * to play for fun.
 */
void Player::toggleInvul()
{
	if (status == Player::Status::Alive)
	{
		status = Player::Status::Invul;
		sprite.setColor(sf::Color(255, 255, 255, 125));
	}
	else if (status == Player::Status::Invul)
	{
		status = Player::Status::Alive;
		sprite.setColor(sf::Color(255, 255, 255));
	}
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

	frame = frame + (1 * DT::dt * DT::mult);
	if (frame >= aniSpeed)
	{
		sprite.setTexture(*textures[1]);
		frame = -aniSpeed;
	}
	else if (frame >= 0)
		sprite.setTexture(*textures[0]);

	if(cooldown && cooldownCounter < shotRate + 3)
		cooldownCounter = cooldownCounter + (1 * DT::dt * DT::mult);

	if (status == Status::Shield)
	{
		if (shieldCounter < shieldDur)
		{
			shieldCounter = shieldCounter + (1 * DT::dt * DT::mult);
			if (shieldCounter < shieldDur - 70)
				sprite.setColor(sf::Color(
					((int)shieldCounter * 5 % 155) + 100,
					((int)shieldCounter * 5 % 255),
					((int)shieldCounter * 2 % 200) + 55, 255));
			else
				sprite.setColor(sf::Color(255, 255, 255, 45 + 3 * (shieldCounter + 70 % (int)shieldDur)));
		}

		else
		{
			status = Status::Alive;
			sprite.setColor(sf::Color(255, 255, 255, 255));
			shieldCounter = 0;

			shotRate = (shotRate == 1) ? 1 : shotRate * 2;
			shotSpeed /= 2;
			velocity /= 1.5;
			baseDamage /= 2;
		}
	}

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

	hitbox.setPosition(getPos());
}

/*
 * "Translate" keyboard inputs into actions.
 */
void Player::updateInput()
{
	bool moving = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		sprite.move(-velocity * DT::dt * DT::mult, 0.f);

		if(sway > -velocity / 4) 
			sway = sway - (.2 * DT::dt * DT::mult);
		//sway = -velocity / 4;
		moving = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		sprite.move(velocity * DT::dt * DT::mult, 0.f);

		if (sway < velocity / 4)
			sway = sway + (.2 * DT::dt * DT::mult);
		//sway = velocity / 4;
		moving = true;
	}

	if (!moving)
	{
		if (sway > -.3 && sway < .3)
			sway = 0;
		else
			sway = (sway < 0) ? sway + (.5 * DT::dt * DT::mult)
							: sway - (.5 * DT::dt * DT::mult);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		sprite.move(0.f, -velocity * DT::dt * DT::mult);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		sprite.move(0.f, velocity * DT::dt * DT::mult);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		if (canAttack())
		{
			bullets.emplace_back(new Bullet(sprite.getPosition().x, 
				sprite.getPosition().y - sprite.getGlobalBounds().height / 2, shotSpeed, continum));
			bullets[bullets.size() - 1]->setSway(sway);
			bullets[bullets.size() - 1]->setLoopLimit(sf::Vector2<short>(loopLimit.x, loopLimit.y));
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
	target.draw(hitbox);

	for (auto* k : bullets)
		k->render(target);
}
