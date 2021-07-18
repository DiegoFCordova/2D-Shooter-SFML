#include "UI.h"

void UI::init()
{
	if (!font.loadFromFile("Fonts/Consolas.ttf"))
	{
		std::cout << "Error: Font not loaded correctly." << "\n";
		return;
	}

	state = MenuState::Main;
	diff = Difficulty::Normal;
	inGame = false;

	choice = 0;

	textsMain.emplace_back(new sf::Text("Start", font, 70));
	textsMain.emplace_back(new sf::Text("Controls", font, 70));
	textsMain.emplace_back(new sf::Text("Options", font, 70));
	textsMain.emplace_back(new sf::Text("Quit", font, 70));

	for(int k = 0; k < 6; k++)
		textsOptions.emplace_back(new sf::Text("Option Placeholder", font, 70));

	optionBase[0] = "Difficulty";
	optionBase[1] = "Shot Rate    ";
	optionBase[2] = "Shot Speed   ";
	optionBase[3] = "Player Speed ";
	optionBase[4] = "Continum  ";
	optionBase[5] = "E-Spawn Rate ";


	textsPause.emplace_back(new sf::Text("Continue", font, 70));
	textsPause.emplace_back(new sf::Text("Options", font, 70));
	textsPause.emplace_back(new sf::Text("Return to Main Menu", font, 70));

	pause = new sf::Text("Pause", font, 210);
	pause->move(510, 75);

	if (!ptrTex.loadFromFile("Sprites/BulletNormal.png"))
	{
		std::cout << "Error: pointer texture not loaded correctly." << "\n";
		return;
	}

	pointer.setTexture(ptrTex);
	pointer.setOrigin(pointer.getOrigin().x + (pointer.getLocalBounds().width / 2), pointer.getOrigin().y + (pointer.getLocalBounds().height / 2));
	pointer.rotate(90);
	pointer.setScale(5,5);

	if(!rulesTex.loadFromFile("Sprites/Controls.png"))
	{
		std::cout << "Error: rules texture not loaded correctly." << "\n";
		return;
	}

	rules.setTexture(rulesTex);
}

/*
 * Default constructor.
 * Should never be used.
 */
UI::UI()
{
	init();
}

UI::UI(float x, float y)
{
	init();
	textsMain[0]->setPosition(x + 6, y);
	textsMain[1]->setPosition(x - 51, y + 80);
	textsMain[2]->setPosition(x - 30, y + 160);
	textsMain[3]->setPosition(x + 26, y + 240);

	textsOptions[0]->setPosition(x - 650, y - 420);
	textsOptions[1]->setPosition(x - 650, y - 340);
	textsOptions[2]->setPosition(x - 650, y - 260);
	textsOptions[3]->setPosition(x - 650, y - 180);
	textsOptions[4]->setPosition(x - 650, y - 100);
	textsOptions[5]->setPosition(x - 650, y - 20);

	textsPause[0]->setPosition(x - 52, y - 20);
	textsPause[1]->setPosition(x - 30, y + 60);
	textsPause[2]->setPosition(x - 262, y + 140);

	hightlight(textsMain);
}

UI::~UI()
{
	for (auto* tm : textsMain)
		delete tm;
	for (auto* to : textsOptions)
		delete to;
	for (auto* tp : textsPause)
		delete tp;
	delete pause;
}

/*
 * Highlights choice and sets
 * pointer next to it.
 * 
 * @param t: Array of Text.
 */
void UI::hightlight(const std::vector<sf::Text*>& t)
{
	for (int k = 0; k < t.size(); k++)
	{
		if(k != choice)
			t[k]->setFillColor(sf::Color(180, 180, 180));
	}

	t[choice]->setFillColor(sf::Color(255, 255, 255));
	pointer.setPosition(t[choice]->getPosition().x - 65, t[choice]->getPosition().y + 45);
}

/*
 * Changes state to given parameter.
 * 
 * @param s: new MenuState.
 */
void UI::setMenuMode(MenuState s)
{
	state = s;
}

/*
 * Handles ESC and BACKWARDS keyboard input
 * to go back to specific state.
 * (For now only affects Controls and Options).
 */
void UI::goBackTo()
{
	if (state == UI::MenuState::Controls || state == UI::MenuState::Options)
	{
		if (inGame)
		{
			state = MenuState::Pause;
			choice = 1;
			hightlight(textsPause);
		}
		else
		{
			choice = (state == UI::MenuState::Controls) ? 1 : 2;
			state = MenuState::Main;
			hightlight(textsMain);
		}
	}
	else if (state == UI::MenuState::Game)
	{
		state = MenuState::Pause;
		hightlight(textsPause);
	}
	else if (state == UI::MenuState::Pause)
	{
		state = MenuState::Game;
		choice = 0;
		hightlight(textsPause);
	}
}

/* Select Up */
void UI::up()
{
	switch (state)
	{
	case UI::MenuState::Main:
		choice = (choice == 0) ? textsMain.size() - 1 : choice - 1;
		hightlight(textsMain);
		break;
	case UI::MenuState::Options:
		choice = (choice == 0) ? textsOptions.size() - 1 : choice - 1;
		hightlight(textsOptions);
		break;
	case UI::MenuState::Pause:
		choice = (choice == 0) ? textsPause.size() - 1 : choice - 1;
		hightlight(textsPause);
		break;
	default:
		break;
	}
}

/* Select Down */
void UI::down()
{
	switch (state)
	{
	case UI::MenuState::Main:
		choice = (choice == textsMain.size() - 1) ? 0 : choice + 1;
		hightlight(textsMain);
		break;
	case UI::MenuState::Options:
		choice = (choice == textsOptions.size() - 1) ? 0 : choice + 1;
		hightlight(textsOptions);
		break;
	case UI::MenuState::Pause:
		choice = (choice == textsPause.size() - 1) ? 0 : choice + 1;
		hightlight(textsPause);
		break;
	default:
		break;
	}
}

int UI::enter()
{
	int ret = choice;

	if (state == MenuState::Main)
	{
		switch (choice)
		{
		case 0:
			state = MenuState::Game;
			inGame = true;
			hightlight(textsMain);
			break;
		case 1:
			state = MenuState::Controls;
			break;
		case 2:
			state = MenuState::Options;
			choice = 0;
			hightlight(textsOptions);
			break;
		case 3:
			return -1;
			break;
		default:
			break;
		}
	}

	else if (state == MenuState::Pause)
	{
		switch (choice)
		{
		case 0:
			state = MenuState::Game;
			break;
		case 1:
			state = MenuState::Options;
			choice = 0;
			hightlight(textsOptions);
			break;
		case 2:
			state = MenuState::Main;
			choice = 0;
			inGame = false;
			///Signal to reset game.
			hightlight(textsMain);
			break;
		default:
			break;
		}
	}
	return ret;
}

/*
 * Method to update Options depending
 * on given value c that represents index
 * of textsOptions array.
 * (c default = -98, if so, c = choice)
 * 
 * @param num: Updated value for at index.
 * @param c: Specify which index gets 
 *				updated with num value.
 */
int UI::optionSet(int num, int c)
{
	std::stringstream str;
	if (c == -98)
		c = choice;
	str << optionBase[c] << "\t\t\t";


	switch (c)
	{
	case 0:
		switch (num) 
		{
		case 0: case 3:
			str << "Easy";
			textsOptions[c]->setString(str.str());
			diff = Difficulty::Easy;
			return 0;
		case 1:
			str << "Normal";
			textsOptions[c]->setString(str.str());
			diff = Difficulty::Normal;
			return 1;
		case 2: case -1:
			str << "Merciless";
			textsOptions[c]->setString(str.str());
			diff = Difficulty::Merciless;
			return 2;
		default:
			std::cout << "Issue with UI.optionSet, Difficulty Choice.\n";
			break;
		}
		break;
	case 1: case 2: case 3: case 5:
		if (num < 1)
			num = 1;
		str << num;
		textsOptions[c]->setString(str.str());
		return num;
		std::cout << "Should never get to here.\n";
		break;
	case 4:
		switch (num)
		{
		case 0: case 4:
			str << "None";
			textsOptions[c]->setString(str.str());
			return 0;
		case 1:
			str << "Horizontal"; 
			textsOptions[c]->setString(str.str());
			return 1;
		case 2:
			str << "Vertical"; 
			textsOptions[c]->setString(str.str());
			return 2;
		case 3: case -1:
			str << "All";
			textsOptions[c]->setString(str.str());
			return 3;
		default:
			std::cout << "Issue with UI.optionSet, Continnum Choice.\n";
			break;
		}
		break;
	default:
		std::cout << "Issue with UI.optionSet, Choice.\n";
		return -1;
	}

	//Just to check
	std::cout << str.str() << "\n";
}

/* @return inGame boolean */
bool UI::isInGame()
{
	return inGame;
}

/*
 * @return Current state in UI.
 */
UI::MenuState UI::getState() const
{
	return state;
}

UI::Difficulty UI::getDifficulty() const
{
	return diff;
}

/*
 * @return choice/index.
 */
int UI::getChoice() const
{
	return choice;
}

/*
 * Updates which option is selected.
 */
void UI::update(sf::RenderTarget& target)
{

}

/*
 * Renders textsMain to target window.
 */
void UI::render(sf::RenderTarget& target)
{
	switch (state)
	{
	case UI::MenuState::Main:
		for (auto* t : textsMain)
			target.draw(*t);
		target.draw(pointer);
		break;
	case UI::MenuState::Controls:
		target.draw(rules);
		break;
	case UI::MenuState::Options:
		//target.draw(options); ///Might be all text
		for (auto* t : textsOptions)
			target.draw(*t);
		break;
	case UI::MenuState::Pause:
		target.draw(*pause);
		for (auto* t : textsPause)
			target.draw(*t);
		target.draw(pointer);
		break;
	default:
		break;
	}
}
