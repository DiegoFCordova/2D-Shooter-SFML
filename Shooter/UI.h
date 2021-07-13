#pragma once

#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Bullet.h"

/*
 * Class for in-game UI.
 */
class UI
{
public:
	enum class MenuState
	{
		Main, Controls, Options, Pause, Game
	};

private:
	std::vector<sf::Text*> texts;
	sf::Texture ptrTex, rulesTex;
	sf::Sprite pointer, rules;
	sf::Font font;
	sf::Text t;
	MenuState state;
	int choice;

	void init();

public:


	UI();
	UI(float x, float y);
	~UI();

	void hightlight();
	void setMenuMode(MenuState s);
	void goBackTo();

	void up();
	void down();
	int enter();

	UI::MenuState getState() const;

	void update(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);
};

