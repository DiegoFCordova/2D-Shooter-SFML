#include "Game.h"

/* 
 * Initialize Variables, called in constructor:
 * -Window
 * -Enemy spawn timer and limit
 * -Max enemy limit
 * -Points
 * --Reserve an size in the vector to avoid unnecesary copying--
 */
void Game::initVars()
{
	window = nullptr;
	enemySpawnTimerMax = 25.f;
	enemySpawnTimer = enemySpawnTimerMax;
	maxEnemies = 50;
	points = 0;

	//enemies.reserve(maxEnemies);
}

/*
 * Called in constructor. Initialize windows, sets
 * titlebar and frames limit.
 */
void Game::initWindow()
{
	vidMode.width = 800;
	vidMode.height = 600;
	window = new sf::RenderWindow(vidMode, "Demo", sf::Style::Titlebar | sf::Style::Close);
	window->setFramerateLimit(144);
}

/*
 * Called in constructor: Might refactor: Initializes "enemy" to certain
 * position colour, size, etc
 */
void Game::initMobs()
{
	enemy.setPosition(400.0, 300.0);
	enemy.setSize(sf::Vector2f(100.0, 100.0));
	enemy.setFillColor(sf::Color(220, 220, 170));
	enemy.setOutlineColor(sf::Color(108, 169, 155));
	enemy.setOutlineThickness(2.0);
}

/*
 * Constructor. Calls 3 init functions, described above
 */
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

//-System (Might choose another name for this group)
/* Returns true if window is open */
const bool Game::running() const
{
	return window->isOpen();
}

/* 
 * Updates game logic:
 * Functions used are described below in order.
 */
void Game::update()
{
	pollEvents();
	updateMousePos();
	updateEnemies();

	//Update mouse position
	///if(mousePosWindow.x > 0 && mousePosWindow.y > 0) //Might, but don't feel like it. For now.
	std::cout << "Mouse Coor (" << enemies.size() << ", " << maxEnemies << ")\n";
}

/*
 * Event Polling loops:
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
			if (ev.key.code == sf::Keyboard::Escape)
				window->close();
			break;
		}
	}
}

/*
 * Updates mouse position on game screen.
 */
void Game::updateMousePos()
{
	mousePosWindow = sf::Mouse::getPosition(*window);
}

/* 
 * Calls spawnEnemy (below) when interval is met,
 * also moves all existing elements in "enemies"
 * vector downwards
 */
void Game::updateEnemies()
{

	//updates the timer for enemy spawning
	if (enemies.size() < maxEnemies)
	{
		if (enemySpawnTimer >= enemySpawnTimerMax)
		{
			spawnEnemy();
			enemySpawnTimer = 0.f;
		}
		else
			enemySpawnTimer += 1.f;
	}

	//Move enemies
//	for (int k = 0; k < maxEnemies; k++)
		//enemies.at(k+1).move(0.f, 5.f);

	for (auto& e : enemies)
		e.move(0.f, 5.f);
}

/*
 * Spawn enemy at a random x position and at the top of the screen
 */
void Game::spawnEnemy()
{
	enemy.setPosition(static_cast<float>(rand() % static_cast<int>(window->getSize().x - enemy.getSize().x)),
		0.f);			///Rewrite
	//enemy.setFillColor(sf::Color(220, 220, 170));

	enemies.push_back(enemy);			///Replace with emplace_back once enemy class is done
}


/*
 * Clears old frame, renders object,
 * and finally display new frame in window
 * Uses:
 * -renderEnemies
 */
void Game::render()
{
	window->clear(sf::Color(30, 30, 30));

	//Draw game Objects
	//window->draw(mob);
	renderEnemies();

	window->display();
}

/*
 * Draw elements from vector "enemies" to window
 */
void Game::renderEnemies()
{
	//Move enemies
	for (int k = 0; k < enemies.size(); k++)
		window->draw(enemies[k]);


}
