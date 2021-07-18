#pragma once

#include <string>
#include <sstream>

#include "Player.h"

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

	enum class Difficulty
	{
		Easy, Normal, Merciless
	};

private:
	std::vector<sf::Text*> textsMain, textsOptions, textsPause;
	sf::Texture ptrTex, rulesTex;
	sf::Sprite pointer, rules;
	sf::Font font;
	sf::Text* pause;
	std::string optionBase[6];
	MenuState state;
	Difficulty diff;
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
	int optionSet(int num, int c = -98);			///, Game::GameState gstate);
	bool isInGame();

	UI::MenuState getState() const;
	UI::Difficulty getDifficulty() const;
	int getChoice() const;

	void update(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);
};

