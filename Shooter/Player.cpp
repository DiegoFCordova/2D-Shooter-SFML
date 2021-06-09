#include "Player.h"

void Player::initVariables()
{

}

void Player::initShape()
{
	body.setFillColor(sf::Color::Red);
	body.setSize(sf::Vector2f(100.f, 100.f));
}

Player::Player()
{
	initVariables();
	initShape();
}

Player::Player(float x, float y)
{
	body.setPosition(x, y);

	initVariables();
	initShape();
}

Player::~Player()
{

}

void Player::update(sf::RenderTarget& target)
{
	updateInput();
}

void Player::updateInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		body.move(-5.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		body.move(5.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		body.move(0.f, -5.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		body.move(0.f, 5.f);
	}
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(body);
}
