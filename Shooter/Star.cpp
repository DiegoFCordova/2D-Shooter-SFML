#include "Star.h"

/*
 * Simple constructor to specify speed.
 */
Star::Star(int x, int y, int size, float spd)
{
	effect = FX::Normal;
	loop = Loop::Normal;
	oriSpd = spd;
	speed = oriSpd;
	sway = 0;
	lap = true;
	frame = 0;
	frameDelay = (int)(rand() % 100);

	star.setSize(sf::Vector2<float>(size, size));
	star.setPosition(x, y);

	if (x % 2 == 0)
	{
		oriAlp = x % 200 + 55;
		alpha = oriAlp;
		r = y % 87;
		g = 116;
		b = 48;
		star.setFillColor(sf::Color(r, g, b, alpha));
	}
	else
	{
		oriAlp = x % 100 + 55;
		alpha = oriAlp;
		r = x % 239;
		g =  242;
		b = x % 132;
		star.setFillColor(sf::Color(r, g, b, alpha));
	}
}
/*
 * No pointer used so far, but
 * will put it here if needed.
 */
Star::~Star()
{
}

/*
 * Simple effect. Star will drop
 * speed and procede to accelerate in the opposite direction.
 */
void Star::inverseSpeedFX()
{
	effect = FX::InverseSpeed;
	resetFXs();
}
/*
 * After reaching certain point, starts
 * going in the oposite direction.
 */
void Star::masaFX()
{
	effect = FX::Masa;
	resetFXs();
}
/*
 * Complex animation goes here.
 */
void Star::masaAnimation(int widthThird)
{
	frame++;
	if (frame > 120 + frameDelay && speed > -(oriSpd * 2))
		speed -= .2;

	else if (frame == 120 + frameDelay)
	{
		star.setFillColor(sf::Color(73, 0, 142, 255));
		star.setOutlineThickness(1);
		star.setOutlineColor(sf::Color(74, 0, 68));
		speed -= .2;
	}

	else if (frame == 119 + frameDelay)
	{
		star.setPosition(widthThird + (int)(rand() % widthThird), 450);
		sway = (frameDelay % 3 == 0) ? 0 
			: (frameDelay % 2 == 0) ? 1 : -1;
		speed = oriSpd;
		loop = Loop::MFirstLoop;
	}

	else if (frame > frameDelay && alpha > 0)
	{
		alpha = (alpha - 30 < 0) ? 0 : alpha -30;
		star.setFillColor(sf::Color(r, g, b, alpha));
	}
}

/*
 * Sets effect to normal.
 */
void Star::normalFX()
{
	effect = FX::Normal;
	resetFXs();
}

/*
 * Set star values to normal.
 */
void Star::resetFXs()
{
	alpha = oriAlp;
	star.setFillColor(sf::Color(r, g, b, alpha));
	star.setOutlineThickness(0);
	loop = Loop::Normal;
	lap = true; 
	frame = 0;
	sway = 0;
}

void Star::update(sf::RenderTarget& target)
{
	int widthThird = target.getSize().x / 3;
	switch (effect)
	{
	case Star::FX::Normal:
		if (speed < oriSpd)
			speed += .5;
		break;
	case Star::FX::InverseSpeed:
		if (speed > -oriSpd)
			speed -= .5;
		break;
	case Star::FX::Masa:
		masaAnimation(widthThird);
		break;
	default:
		break;
	}

	star.move(sway, speed);


	if (effect == FX::Masa && speed < 0 && star.getPosition().y < 0)
	{
		if (lap)
		{
			if (loop == Loop::MFirstLoop)
			{
				star.setPosition(widthThird + (int)(rand() % widthThird), 455);
				loop = Loop::MLap;
			}
			else if (loop == Loop::MLap)
			{
				star.setPosition((int)(rand() % target.getSize().x), (int)(rand() % target.getSize().y));
				lap = false;
			}
			speed = oriSpd;
		}
		else
		{
			star.setPosition(star.getPosition().x, target.getSize().y);
			lap = true;
		}
	}

	else
	{
		if (star.getPosition().y > target.getSize().y)
			star.setPosition((int)(rand() % target.getSize().x), 0);
		else if (speed < 0 && star.getPosition().y < 0)
			star.setPosition(star.getPosition().x, target.getSize().y);
	}
}

void Star::render(sf::RenderTarget& target)
{
	target.draw(star);
}
