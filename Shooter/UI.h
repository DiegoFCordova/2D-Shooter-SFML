#pragma once

#include <string>

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
	std::vector<sf::Text*> textsMain, textsOptions, textsPause;
	sf::Texture ptrTex, rulesTex;
	sf::Sprite pointer, rules;
	sf::Font font;
	sf::Text t;
	MenuState state;
	int choice;
	bool inGame;

	void init();

public:


	UI();
	UI(float x, float y);
	~UI();

	void hightlight(const std::vector<sf::Text*>& t);
	void setMenuMode(MenuState s);
	void goBackTo();

	void up();
	void down();
	int enter();
	bool isInGame();

	UI::MenuState getState() const;

	void update(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);
};

