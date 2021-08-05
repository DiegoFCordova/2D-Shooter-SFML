#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "DT.h"

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
	float oriSpd, speed, sway, frame;
	int oriAlp, alpha, frameDelay;
	short r, g, b, masaStarStage;
	bool lap, readyToChange;

	void masaAnimation(int widthThird);

public:
	Star(int x, int y, int size, float spd);
	~Star();

	void inverseSpeedFX();
	void masaFX();
	void normalFX();
	void resetFXs();

	void blinkFX(int blinkInt);

	Star::FX getFX() const;
	bool isReadyToChange() const;

	void update(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);
};
