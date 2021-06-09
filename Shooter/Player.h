#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Player
{
private:
	sf::RectangleShape body;

	void initVariables();
	void initShape();

public:
	Player();
	Player(float x, float y);
	virtual ~Player();

	void update(sf::RenderTarget& target);
	void updateInput();
	void render(sf::RenderTarget& target);
};
