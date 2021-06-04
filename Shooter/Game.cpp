#include "Game.h"

/* Initialize pointers */
void Game::initVars()
{
	window = nullptr;

}

/* Initialize windows */
void Game::initWindow()
{
	vidMode.width = 800;
	vidMode.height = 600;
	window = new sf::RenderWindow(vidMode, "Demo", sf::Style::Titlebar | sf::Style::Close);
	window->setFramerateLimit(144);
}

void Game::initMobs()
{
	mob.setPosition(400.0, 300.0);
	mob.setSize(sf::Vector2f(100.0, 100.0));
	mob.setFillColor(sf::Color(220, 220, 170));
	mob.setOutlineColor(sf::Color(108, 169, 155));
	mob.setOutlineThickness(2.0);
}

/* Constructor */
Game::Game()
{
	initVars();
	initWindow();
	initMobs();
}

/* Destructor */
Game::~Game()
{
	delete window;
}


//-Accessors
/* Returns true if window is open */
const bool Game::running() const
{
	return window->isOpen();
}

/*
 * Event Polling loops
 */
void Game::pollEvents()
{
	while (window->pollEvent(ev))
	{
		switch (ev.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape)
				window->close();
			break;
		}
	}
}

//-Functions
/* Updates game logic */
void Game::update()
{
	pollEvents();

	//Update mouse position
	///if(sf::Mouse::getPosition(*window).x > 0 && sf::Mouse::getPosition(*window).y > 0) //Might, but don't feel like it. For now.
	std::cout << "Mouse Coor (" << sf::Mouse::getPosition(*window).x
		<< ", " << sf::Mouse::getPosition(*window).y << ")\n";
}

/* 
 * Clears old frame, renders object, 
 * and finally display new frame in window
 */
void Game::render()
{
	window->clear(sf::Color(30, 30, 30));

	//Draw game Objects
	window->draw(mob);

	window->display();
}
