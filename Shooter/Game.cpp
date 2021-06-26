#include "Game.h"

/*
 * Initialize windows, sets
 * titlebar, and frames limit.
 */

///Quick dirty code to debug
bool debug = false;

void Game::initWindow()
{
	vidMode.width = 1600;
	vidMode.height = 900;
	window = new sf::RenderWindow(vidMode, "Demo", sf::Style::Titlebar | sf::Style::Close);
	window->setFramerateLimit(144);
}

/*
 * Initialize game logic variables.
 */
void Game::initVars()
{
	state = State::MainMenu;
	tileSize = (float)(window->getSize().x) / 32;	//For  16:9 ratio

	enemySpawnTimerMax = 25.f;
	enemySpawnTimer = enemySpawnTimerMax;
	maxEnemies = 50;
	points = 0;
}

/*
 * Initialize Player and a certain number of enemies.
 * --Reserve an size in the vector to avoid unnecesary copying--
 */
void Game::initMobs()
{
	player = new Player(tileSize*16, tileSize*15, 1);
	enemies.reserve(maxEnemies);
	death.reserve(maxEnemies);
}

/* Debugging.
 * Initializes font for text.
 */
void Game::initText()
{
	if (!font.loadFromFile("Fonts/Consolas.ttf"))
	{
		std::cout << "Error: Font not loaded correctly." << "\n";
		return;
	}

	text.setFont(font);
	text.setCharacterSize(tileSize*.85);
	text.setFillColor(sf::Color(218, 218, 218));
	text.setString("TEXT GOES HERE");
}


/*
 * Constructor. Calls init methods, described above.
 */
Game::Game()
{
	initWindow();
	initVars();
	initText();
	initMobs();
}

/* Destructor */
Game::~Game()
{
	delete window;
	delete player;
	for (auto* e : enemies)
		delete e;
	for (auto* d : death)
		delete d;
}


/* Returns true if window is open */
const bool Game::running() const
{
	return window->isOpen();
}

/* 
 * Updates game logic for next frame.
 * Rest of the methods are described below.
 */
void Game::update()
{
	pollEvents();
	//if(state == State::Game)
		updateMobs();

	//-Debug
	updateDebug();
}

/*
 * Event Polling loop:		///Might only use for UI or not at all besides window's interactions
 * -If windows is closed
 * -Gets what key is pressed
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
			if (ev.key.code == sf::Keyboard::Escape)	//UI's Exit should trigger this
				window->close();
			//else if (ev.key.code == sf::Keyboard::Z)		///Za Warudo, pero mejor le bajamos la velocidad a todos los mobs?
			//	window->setFramerateLimit(60);

			///Debug quick dirty code
			else if (ev.key.code == sf::Keyboard::Z)
				debug = !debug;
			else if (ev.key.code == sf::Keyboard::Q)
				enemies.emplace_back(new Enemy(rand() % vidMode.width, rand() % (vidMode.height/2)));
			else if (ev.key.code == sf::Keyboard::E)
				enemies.emplace_back(new Enemy(vidMode.width / 2, vidMode.height / 2));
			else if (ev.key.code == sf::Keyboard::R)
				death.emplace_back(new DeathAni(rand() % vidMode.width, rand() % vidMode.height, 10));
			else if (ev.key.code == sf::Keyboard::F)
				for (auto *e : enemies)
				{
					e->attack(player->getPos().x, player->getPos().y);
				}
				break;
		}
	}
}

/* 
 * Updates Mobs movement only if state
 * is "Game".
 * Calculates intersection.
 * ---Hitboxes not done yet---
 */
void Game::updateMobs()
{
	player->update(*window);

	//Loop for player bullets against enemies
	for (int k = 0; k < player->getBullets().size(); k++)
	{
		if (!player->getBullets()[k]->isActive())
		{
			delete player->getBullets()[k];
			player->getBullets().erase(player->getBullets().begin() + k);
		}

		else
		{
			for (int e = 0; e < enemies.size() && enemies[e]->isAlive(); e++)
			{

				if (enemies[e]->bounds().intersects(player->getBullets()[k]->bounds()))
				{
					enemies[e]->takeDamage(player->damageDealt(k));
					player->getBullets()[k]->deactivate();

					if (!enemies[e]->isAlive())
					{
						death.emplace_back(new DeathAni(enemies[e]->getPos().x, enemies[e]->getPos().y, enemies[e]->getLargestSide()));
						delete enemies[e];
						enemies.erase(enemies.begin() + e);

						///Maybe do a bullet hit explotion animation
						delete player->getBullets()[k];
						player->getBullets().erase(player->getBullets().begin() + k);

						points++;
					}
					break;
				}
			}
		}
	}

	//Loop for enemies update and their bullets against player
	for (int e = 0; e < enemies.size(); e++)
	{
		enemies[e]->update(*window);

		for (int k = 0; k < enemies[e]->getBullets().size(); k++)
		{
			if (!enemies[e]->getBullets()[k]->isActive())
			{
				delete enemies[e]->getBullets()[k];
				enemies[e]->getBullets().erase(enemies[e]->getBullets().begin() + k);
			}

			else if (player->isAlive() && player->bounds().intersects(enemies[e]->getBullets()[k]->bounds()))
			{
				player->takeDamage(enemies[e]->damageDealt(k));

				if (!player->isAlive())
				{
					death.emplace_back(new DeathAni(player->getPos().x, player->getPos().y, player->getLargestSide()));
					//Lifes--, respawn animations and such.
				}

				///Same as before, if you do Bullet explotion animation, add it here as well.
				delete enemies[e]->getBullets()[k];
				enemies[e]->getBullets().erase(enemies[e]->getBullets().begin() + k);
			}
		}

	}

	for (int d = 0; d < death.size(); d++)
	{
		if (death[d]->isOver())
		{
			delete death[d];
			death.erase(death.begin() + d);
		}
		else
			death[d]->update(*window);
	}
}

/*
 * Refreshes windows to next frame
 * and render all active objects.
 */
void Game::render()
{
	window->clear(sf::Color(30, 30, 30));
	renderMobs();

	//Debug
	if (debug)
	{
		renderDebug(*window);
		drawGrid();
	}

	window->display();
}

/* Render mobs after update is done */
void Game::renderMobs()
{
	player->render(*window);
	for (auto* k : enemies)
		k->render(*window);
	for (auto* d : death)
		d->render(*window);
}


//-Debug

/* Update debug components */
void Game::updateDebug()
{
	std::stringstream str;

	str << "Points:" << points
		<< "\nX: " << player->getPos().x << " Y: " << player->getPos().y
		<< "\nBullets: " << player->bulletsCreated()
		<< "\nEnemies: " << enemies.size()
		<< "\nTileSize: " << tileSize
		<< "\nPlayer hp: " << player->getHP() << ", alive: " << player->isAlive()
		<< "\nWidth (Local): " << player->sprite.getLocalBounds().width
		<< "\nHeight (Local): " << player->sprite.getLocalBounds().height
		<< "\nWidth (Global): " << player->sprite.getGlobalBounds().width
		<< "\nHeight (Global): " << player->sprite.getGlobalBounds().height;

	text.setString(str.str());


	///Trash code to check all enemies shooting per c frames.
	int static c = 0;
	c++;
	if (c % 20 == 0)
	{
		for (auto* e : enemies)
		{
			e->attack(player->getPos().x, player->getPos().y);
		}
	}
		//enemies.emplace_back(new Enemy(rand() % vidMode.width, rand() % vidMode.height));
}

/* Render debug components after update is done */
void Game::renderDebug(sf::RenderTarget& target)
{
	target.draw(text);
}

/*
 * Draws grid for easy organization
 */
void Game::drawGrid()
{
	sf::VertexArray linesVertical(sf::Lines, 66);
	sf::VertexArray linesHorizontal(sf::Lines, 36);

	for (int k = 0; k < 66; k += 2)
	{
		//Middle
		if (k == 32)
		{
			linesVertical[k].color = sf::Color::Red;
			linesVertical[k + 1].color = sf::Color::Red;
		}

		//Quarter
		else if (k % 8 == 0)
		{
			linesVertical[k].color = sf::Color::Yellow;
			linesVertical[k + 1].color = sf::Color::Yellow;
		}

		linesVertical[k].position = sf::Vector2f(tileSize * (k / 2), 0);
		linesVertical[k + 1].position = sf::Vector2f(tileSize * (k / 2), window->getSize().y);
	}

	for (int k = 0; k < 36; k += 2)
	{
		if (k == 18)
		{
			linesHorizontal[k].color = sf::Color::Red;
			linesHorizontal[k + 1].color = sf::Color::Red;
		}

		else if (k % 6 == 0)
		{
			linesHorizontal[k].color = sf::Color::Yellow;
			linesHorizontal[k + 1].color = sf::Color::Yellow;
		}

		linesHorizontal[k].position = sf::Vector2f(0, tileSize * (k / 2));
		linesHorizontal[k + 1].position = sf::Vector2f(window->getSize().x, tileSize * (k / 2));
	}

	window->draw(linesVertical);
	window->draw(linesHorizontal);
}
