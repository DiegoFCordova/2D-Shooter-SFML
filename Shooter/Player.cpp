#include "Player.h"

/*
 * Initialize basic variables to default for player.
 */
void Player::initVariables()
{
	bullets.reserve(50);
	shotRate = 14;
	counter = 0;
	cooldown = false;
	velocity = 5;
	sway = 0;
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
	if (!texture.loadFromFile("Sprites/Ship.png"))
		std::cout << "Error loading Player Sprite.\n";
	sprite.setTexture(texture);
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
 * ///Maybe make it so fire becomes true
 */
Player::Player(float x, float y, float scaling)
{
	initVariables();
	initSprite();
	sprite.setPosition(x, y);
}

/*
 * Destructor for pointers.
 */
Player::~Player()
{
	for (auto *k : bullets)
		delete k;
}

/*
 * sprite position getter.
 * @return Vector2 containing x and y of sprite in the window.
 */
sf::Vector2<float> Player::getPos() const
{
	return sprite.getPosition();
}

/*
 * @return sprite's global bounds.
 */
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
		if (counter >= shotRate)
			cooldown = false;
		return false;
	}

	else
	{
		cooldown = true;
		counter = 0;
		return true;
	}
}

/*
 * Calculates how much damage the bullet makes.
 */
float Player::damageDealt(int k)
{
	return bullets[k]->atk();		//Add damage multiplier later
}

/*
 * Update animations.
 * Also loops player if he goes outside screen.
 */
void Player::update(sf::RenderTarget& target)
{
	float x = sprite.getPosition().x, y = sprite.getPosition().y,
		width = sprite.getLocalBounds().width * scaling, height = sprite.getLocalBounds().height * scaling;
	int targetWidth = target.getSize().x, targetHeight = target.getSize().y;

	if(cooldown)
		counter++;
	updateInput();

	for (auto* k : bullets)
		k->update(target);

	if (y < 0 - height)
		sprite.setPosition(x, targetHeight);
	else if (y > targetHeight)
		sprite.setPosition(x, 0 - height);
	else if (x < 0 - width)
		sprite.setPosition(targetWidth, y);
	else if (x > targetWidth)
		sprite.setPosition(0 - width, y);
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
			bullets.emplace_back(new Bullet(sprite.getPosition().x + (sprite.getGlobalBounds().width/2), sprite.getPosition().y, sway));
}

/*
 * Draws sprite and its objects into target.
 * 
 * @param target: Destination of drawing.
 */
void Player::render(sf::RenderTarget& target)
{
	target.draw(sprite);

	//bullet->render(target);
	for (auto* k : bullets)
		k->render(target);
}
