#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

/*
 * Star class for pretty background.
 *
 * @author Diego Cordova
 */

class Star
{
public:
	enum class FX
	{
		Normal, InverseSpeed, Masa
	};

	enum class Loop
	{
		Normal, MLap, MFirstLoop
	};

private:
	sf::RectangleShape star;
	FX effect;
	Loop loop;
	float oriSpd, speed, sway;
	int oriAlp, alpha,
		frame, frameDelay;
	short r, g, b;
	bool lap;

public:
	Star(int x, int y, int size, float spd);
	~Star();

	void inverseSpeedFX();
	void masaFX();
	void masaAnimation(int widthThird);
	void normalFX();
	void resetFXs();

	void update(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);
};
