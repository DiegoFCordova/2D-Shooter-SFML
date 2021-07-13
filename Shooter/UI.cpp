#include "UI.h"

void UI::init()
{
	if (!font.loadFromFile("Fonts/Consolas.ttf"))
	{
		std::cout << "Error: Font not loaded correctly." << "\n";
		return;
	}

	state = MenuState::Main;

	choice = 0;

	texts.emplace_back(new sf::Text("Start", font, 70));
	texts.emplace_back(new sf::Text("Controls", font, 70));
	texts.emplace_back(new sf::Text("Options", font, 70));
	texts.emplace_back(new sf::Text("Quit", font, 70));

	for (int k = 0; k < texts.size(); k++)
		texts[k]->setFillColor(sf::Color(180, 180, 180));

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
	texts[0]->setPosition(x + 6, y);
	texts[1]->setPosition(x - 51, y + 80);
	texts[2]->setPosition(x - 30, y + 160);
	texts[3]->setPosition(x + 26, y + 240);

	hightlight();
}

UI::~UI()
{
	for (auto* t : texts)
		delete t;
}

/*
 * Highlights choice and sets
 * pointer next to it.
 * 
 * @param c: Choice index in array.
 */
void UI::hightlight()
{
	for (int k = 0; k < texts.size(); k++)
	{
		if(k != choice)
			texts[k]->setFillColor(sf::Color(180, 180, 180));
	}

	texts[choice]->setFillColor(sf::Color(255, 255, 255));
	pointer.setPosition(texts[choice]->getPosition().x - 65, texts[choice]->getPosition().y + 45);
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
 * (For now only affects Controls and Options.
 */
void UI::goBackTo()
{
	if (state == UI::MenuState::Controls || state == UI::MenuState::Options)
		state = MenuState::Main;
	else if (state == UI::MenuState::Game)
		state = MenuState::Pause;
}

/* Select Up */
void UI::up()
{
	choice = (choice == 0) ? texts.size() - 1 : choice - 1;
	hightlight();
}

/* Select Down */
void UI::down()
{
	choice = (choice == texts.size() - 1) ? 0 : choice + 1;
	hightlight();
}

int UI::enter()
{
	if (choice == 0)
		state = MenuState::Game;
	else if (choice == 1)
		state = MenuState::Controls;
	else if (choice == 2)
		state = MenuState::Options;
	return choice;
}

/*
 * @return Current state in UI.
 */
UI::MenuState UI::getState() const
{
	return state;
}

/*
 * Updates which option is selected.
 */
void UI::update(sf::RenderTarget& target)
{

}

/*
 * Renders texts to target window.
 */
void UI::render(sf::RenderTarget& target)
{
	switch (state)
	{
	case UI::MenuState::Main:
		for (auto* t : texts)
			target.draw(*t);
		target.draw(pointer);
		break;
	case UI::MenuState::Controls:
		target.draw(rules);
		break;
	case UI::MenuState::Options:
		//target.draw(options); ///Might be all text
		break;
	case UI::MenuState::Pause:
		///vector of 3 strings and a png.
		break;
	default:
		break;
	}
}
