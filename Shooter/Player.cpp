#include "Player.h"

/*
 * Initialize basic variables to default for player.
 */
void Player::initVariables()
{
	bullets.reserve(50);
	shotRate = 14;
	cooldownCounter = 0;
	cooldown = false;
	frame = 0;
	alive = true;
	velocity = 4;
	sway = 0;
	maxHP = 10;
	hp = maxHP;
	maxBullets = 10;
	scaling = 1.5;
}

/*
 * texture loads certain image and sets it for sprite.
 * Checks if texture loading failed, if it does,
 * print message and return.
 */
void Player::initSprite()
{
	if (!tex1.loadFromFile("Sprites/Ship0.png"))
		std::cout << "Error loading Player Sprite 1.\n";
	if (!tex2.loadFromFile("Sprites/Ship1.png"))
		std::cout << "Error loading Player Sprite 2.\n";
	sprite.setTexture(tex1);
	sprite.scale(scaling, scaling);
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
Player::Player(float x, float y, float scaling)
{
	initVariables();
	initSprite();

	/*
	* 	sf::Vector2<float> center;
	center.x = sprite.getPosition().x + (sprite.getGlobalBounds().width / 2);
	center.y = sprite.getPosition().y + (sprite.getGlobalBounds().height / 2);
	return center;
	*/
	sprite.setOrigin(sprite.getPosition().x + (sprite.getLocalBounds().width / 2), sprite.getPosition().y + (sprite.getLocalBounds().height / 2));

	sprite.setPosition(x, y);
}

/* Destructor for pointers. */
Player::~Player()
{
	for (auto *k : bullets)
		delete k;
}

/* @return Remaining HP of player. */
float Player::getHP() const
{
	return hp;
}

/* @return Max HP of player. */
short Player::getMaxHP() const
{
	return maxHP;
}

/* @return true if player is alive. */
bool Player::isAlive() const
{
	return alive;
}

/*
 * sprite position getter.
 * 
 * @return Vector2 containing x and y of sprite in the window.
 */
sf::Vector2<float> Player::getPos() const
{
	return sprite.getPosition();
}

/* @return sprite's global bounds. */
sf::FloatRect Player::bounds() const
{
	return sprite.getGlobalBounds();
}

/*
 * @return Player's bullets (Would it be better to make bullets public?)
 */
std::vector<Bullet*>& Player::getBullets()
{
	return bullets;
}

/// Delete later
float Player::see()
{
	return sway;
}

/*
 * @return Number of active bullets on screen.
 */
int Player::bulletsCreated()
{
	return bullets.size();
}
/*
 * Attacks if cooldown is false, then makes it true.
 * If cooldown is true, counter increases.
 * When counter reaches shotRate, player can attack again.
 */
bool Player::canAttack()
{
	if (bullets.size() > maxBullets)
		return false;

	if (cooldown)
	{
		if (cooldownCounter >= shotRate)
			cooldown = false;
		return false;
	}

	else
	{
		cooldown = true;
		cooldownCounter = 0;
		return true;
	}
}

/*
 * Calculates how much damage the bullet makes.
 * 
 * @param k: Index of Bullet in bullets vector.
 */
float Player::damageDealt(int k)
{
	return bullets[k]->atk();		///Add damage multiplier later
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
		alive = false;
}

/*
 * Method to save a few accesses for death
 * animation.
 *
 * @return width/height of sprite (Global)
 */
float Player::getLargestSide()
{
	return (sprite.getGlobalBounds().width > sprite.getGlobalBounds().height) ? sprite.getGlobalBounds().width : sprite.getGlobalBounds().height;
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
	if (frame == 60)
	{
		sprite.setTexture(tex2);
		frame = -60;
	}
	else if (frame == 0)
		sprite.setTexture(tex1);

	if(cooldown)
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

		if(sway > -velocity/4) 
			sway -= .1;
		moving = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		sprite.move(velocity, 0.f);

		if (sway < velocity/4)
			sway += .1;
		moving = true;
	}

	if (!moving)
	{
		if (sway > -.1 && sway < .1)
			sway = 0;
		else
			sway += (sway < 0) ? .1 : -.1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		sprite.move(0.f, -velocity);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		sprite.move(0.f, velocity);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		if(canAttack())
			bullets.emplace_back(new Bullet(sprite.getPosition().x , sprite.getPosition().y - sprite.getGlobalBounds().height/2, sway));
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
