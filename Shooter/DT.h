#pragma once
#include <iostream>
#include <SFML/System/Clock.hpp>

class DT
{
private:
	sf::Clock clock;

public:
	static float dt, mult;

	void updateClock();
};

