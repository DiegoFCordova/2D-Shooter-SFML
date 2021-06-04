#include "Game.h"

/* Initialize pointers */
void Game::initVars()
{
	this->window = nullptr;

}

/* Initialize windows */
void Game::initWindow()
{
	this->vidMode.width = 800;
	this->vidMode.height = 600;
	this->window = new sf::RenderWindow(this->vidMode, "Demo", sf::Style::Titlebar | sf::Style::Close);
}

/* Constructor */
Game::Game()
{
	this->initVars();
	this->initWindow();
}

/* Destructor */
Game::~Game()
{
	delete this->window;
}


//-Accessors
/* Returns true if window is open */
const bool Game::running() const
{
	return this->window->isOpen();
}

/*
 * Event Polling loops
 */
void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}

//-Functions
/* Updates game logic */
void Game::update()
{
	this->pollEvents();
}

/* 
 * Clears old frame, renders object, 
 * and finally display new frame in window
 */
void Game::render()
{
	this->window->clear(sf::Color(30, 30, 30));

	//Draw game Objects

	this->window->display();
}
