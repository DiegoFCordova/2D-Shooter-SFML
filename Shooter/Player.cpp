#include "Player.h"

/*
 * Initialize basic variables to default for player.
 *
 * @author MellamoSteve
 */
void Player::initVariables()
{
	bullet = new Bullet();
}

/*
 * texture loads certain image and sets it for sprite.
 * Checks if texture loading failed, if it does,
 * print message and return.
 */
void Player::initSprite()
{
	if (!texture.loadFromFile("Sprites/Test.png"))
		std::cout << "Error loading Player Sprite.\n";
	sprite.setTexture(texture);
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
	delete bullet;
}

/*
 * sprite position getter.
 * @return Vector2 containing x and y of sprite in the window.
 */
sf::Vector2<float> Player::getPos()
{
	return sprite.getPosition();
}

/*
 * Update animations.
 * ///Should use a for loop for bullets
 */
void Player::update(sf::RenderTarget& target)
{
	updateInput();
	bullet->update(target);
}

/*
 * "Translate" keyboard inputs into actions.
 */
void Player::updateInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		sprite.move(-5.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		sprite.move(5.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		sprite.move(0.f, -5.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		sprite.move(0.f, 5.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		bullet = new Bullet(sprite.getPosition().x, sprite.getPosition().y);
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

	bullet->render(target);
}
