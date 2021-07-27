#include "Star.h"

/*
 * Simple for Star.
 * 
 * @param x: X location.
 * @param y: Y location.
 * @param size: Size of the star.
 * @param spd: Vertical speed.
 */
Star::Star(int x, int y, int size, float spd)
{
	effect = FX::Normal;
	loop = Loop::Normal;
	oriSpd = spd;
	speed = oriSpd;
	sway = 0;
	lap = true;
	readyToChange = true;
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
 * Starts complex animation: Masa.
 */
void Star::masaFX()
{
	effect = FX::Masa;
	resetFXs();
	readyToChange = false;
}
/*
 * Complex animation goes here.
 * 
 * @param widthThird: 1 third of the window width.
 */
void Star::masaAnimation(int widthThird)
{
	frame++;
	
	if(frame > 440)
		readyToChange = true;

	if (frame > 120 + frameDelay && speed > -(oriSpd * 2))
		speed -= .2;

	else if (frame == 120 + frameDelay)
	{
		star.setFillColor(sf::Color(73, 0, 142, 155));
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
	star.setScale(1, 1);
	loop = Loop::Normal;
	lap = true; 
	readyToChange = true;
	frame = 0;
	sway = 0;
}

/*
 * Makes it seem as if the star were "blinking".
 * 
 * @param blinkInt: Helps with the bliking effect on the star.
 */
void Star::blinkFX(int blinkInt)
{
	if (effect != FX::Masa)
	{
		if (blinkInt % (oriAlp * (frameDelay + 1 % 12)) < 5)
			star.setFillColor(sf::Color(r, g, b, 0));
		else
			star.setFillColor(sf::Color(r, g, b, oriAlp));
	}
}

/*
 * @return Current FX state of Star.
 */
Star::FX Star::getFX() const
{
	return effect;
}

/*
 * @return True if animation is ready to change.
 */
bool Star::isReadyToChange() const
{
	return readyToChange;
}

/*
 * Updates star movements depending on parameters.
 * 
 * @param target: Window where it will be displayed.
 */
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
				star.setScale(sf::Vector2<float>(2, 2));
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

/*
 * Draws star onto window.
 */
void Star::render(sf::RenderTarget& target)
{
	target.draw(star);
}
