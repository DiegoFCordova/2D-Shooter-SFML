#include "Mob.h"

void Mob::initVariables()
{
	hp = 100;
}

void Mob::initSprite()
{
	if (!texture.loadFromFile("Sprites/Enemy.png"))
		std::cout << "Error loading Player Sprite.\n";
	sprite.setTexture(texture);

	sprite.scale(3, 3);
}

Mob::Mob()
{
	initVariables();
	initSprite();
}

Mob::Mob(float x, float y)
{
	initVariables();
	initSprite();
	sprite.setPosition(x, y);
}

Mob::~Mob()
{

}

void Mob::update(sf::RenderTarget& target)
{
	if (sprite.getPosition().x > target.getSize().x)
		sprite.setPosition(0, sprite.getPosition().y);
	else
		sprite.move(5, 0);
}

void Mob::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
