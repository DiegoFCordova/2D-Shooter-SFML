#include "Game.h"

/*
 * Initialize windows, sets
 * titlebar, frames limit, and game logic.
 */

void Game::initWindow()
{
	vidMode.width = 1600;
	vidMode.height = 900;
	window = new sf::RenderWindow(vidMode, "Medjed", sf::Style::Titlebar | sf::Style::Close);
	window->setFramerateLimit(60);
}

/*
 * Initialize game logic variables.
 */
void Game::initVars()
{
	state = GameState::Opening;
	debug = false;
	tileSize = (float)(vidMode.width) / 32;	//For  16:9 ratio

	frame = 0;
	enemySpawnRate = 25;
	maxEnemies = 100;
	score = 0;

	//For Opening sequence
	blackscreen.setSize(sf::Vector2<float>(vidMode.width, vidMode.height));
	blackscreen.setFillColor(sf::Color(0, 0, 0, 255));
	
	if (!titleTex.loadFromFile("Sprites/Title.png"))
	{
		std::cout << "Error loading Title Sprite.\n";
		return;
	}

	title.setTexture(titleTex);
	title.setPosition(450, 100);
	title.setColor(sf::Color(255, 255, 255, 128));

	//For Main Menu
	ui = new UI(700, 450);
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

/*
 * Background stars are initialized here.
 */
void Game::initStars()
{
	stars.reserve(400);
	for (int k = 0; k < 400; k++)
	{
		int randX = (int)(rand() % vidMode.width),
			randY = (int)(rand() % (vidMode.height));
		stars.emplace_back(new Star(randX, randY, tileSize / 10, 5));
	}
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
	initStars();
}

/* Destructor */
Game::~Game()
{
	delete window;
	delete player;
	delete ui;
	for (auto* e : enemies)
		delete e;
	for (auto* d : death)
		delete d;
	for (auto* s : stars)
		delete s;
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

	if (state == GameState::Opening)
	{
		frame++;
		if (frame == 127)
			state = GameState::MainMenu;
	}

	//if(state == State::Game)
	updateMobs();

	updateStars();

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
			if (ev.key.code == sf::Keyboard::Escape || ev.key.code == sf::Keyboard::BackSpace)	//UI's Exit should trigger this
			{
				if (ui->getState() == UI::MenuState::Main)
					window->close();
				else if (ui->getState() == UI::MenuState::Game)
					state = GameState::PauseMenu;
				ui->goBackTo();
			}
			//else if (ev.key.code == sf::Keyboard::Z)		///Za Warudo, pero mejor le bajamos la velocidad a todos los mobs?
			//	window->setFramerateLimit(60);

			//UI Controls
			else if (ev.key.code == sf::Keyboard::W || ev.key.code == sf::Keyboard::Up)
				ui->up();
			else if (ev.key.code == sf::Keyboard::S || ev.key.code == sf::Keyboard::Down)
				ui->down();
			else if (ev.key.code == sf::Keyboard::Enter)
			{
				switch (ui->enter())
				{
				case 0:
					state = GameState::Game;
					break;
				case 3:
					window->close();
				default:
					break;
				}
			}


			///Debug quick dirty code
			else if (ev.key.code == sf::Keyboard::Z)
				debug = !debug;
			else if (ev.key.code == sf::Keyboard::T)
			{
				for (auto* s : stars)
					s->inverseSpeedFX();
			}
			else if (ev.key.code == sf::Keyboard::Y)
			{
				for (auto* s : stars)
					s->normalFX();
			}
			else if (ev.key.code == sf::Keyboard::G)
			{
				for (auto* s : stars)
					s->masaFX();
			}
			else if (ev.key.code == sf::Keyboard::F)
			{
				frame = 0;
				state = GameState::Opening;
			}
			else if (ev.key.code == sf::Keyboard::Q)
				enemies.emplace_back(new Enemy(rand() % vidMode.width, rand() % (vidMode.height/2)));
			else if (ev.key.code == sf::Keyboard::E)
				enemies.emplace_back(new Enemy(vidMode.width / 2, vidMode.height / 2));
			else if (ev.key.code == sf::Keyboard::R)
				death.emplace_back(new DeathAni(rand() % vidMode.width, rand() % vidMode.height, 10));
		}
	}
}

/* 
 * Updates Mobs movement only if state
 * is "Game", calculates intersection and
 * others in the follwing order:
 *	-Updates Player.
 *  -Check all bullets: If they are off-screen or hit an active enemy.
 *  -Check all enemies: Updates them and check their bullets if they hit player.
 *  -Update all death animations if any and erase them once they are finish.
 */
void Game::updateMobs()
{
	player->update(*window);

	//Loop for player bullets against enemies
	for (int k = 0; k < player->getBullets().size(); k++)
	{
		if (!player->getBullets()[k]->isActive())
			deleteBulletOf(player, k);

		else
		{
			for (int e = 0; e < enemies.size(); e++)
			{

				if (enemies[e]->isActive() && enemies[e]->bounds().intersects(player->getBullets()[k]->bounds()))
				{
					enemies[e]->takeDamage(player->damageDealt(k));
					player->getBullets()[k]->deactivate();

					if (!enemies[e]->isActive())
					{
						death.emplace_back(new DeathAni(enemies[e]->getPos().x, enemies[e]->getPos().y, enemies[e]->getLargestSide()));
						
						if (enemies[e]->activeBullets() == 0)
							deleteMob(&enemies, enemies[e], e);
						else
							enemies[e]->setWaitForDisposal();
						score++;
					}

					death.emplace_back(new DeathAni(player->getBullets()[k]->getPos().x, player->getBullets()[k]->getPos().y,
														player->getBullets()[k]->getLargestSide()*2, DeathAni::Type::Bullet));
					deleteBulletOf(player, k);
					break;
				}
			}
		}
	}

	//Loop for enemies update and their bullets against player
	for (int e = 0; e < enemies.size(); e++)
	{							//Check for dead and not bullets on field here.
		if (!enemies[e]->isActive() && enemies[e]->activeBullets() == 0)
			deleteMob(&enemies, enemies[e], e);

		else
		{
			enemies[e]->update(*window);

			for (int k = 0; k < enemies[e]->getBullets().size(); k++)
			{
				if (!enemies[e]->getBullets()[k]->isActive())
					deleteBulletOf(enemies[e], k);

				else if (player->isActive() && player->bounds().intersects(enemies[e]->getBullets()[k]->bounds()))
				{
					player->takeDamage(enemies[e]->damageDealt(k));

					if (!player->isActive())
					{
						death.emplace_back(new DeathAni(player->getPos().x, player->getPos().y, player->getLargestSide(), DeathAni::Type::Player));
						//Lifes--, respawn animations and such.
					}

					death.emplace_back(new DeathAni(enemies[e]->getBullets()[k]->getPos().x, enemies[e]->getBullets()[k]->getPos().y,
														enemies[e]->getBullets()[k]->getLargestSide() * 2, DeathAni::Type::Bullet));
					death[death.size() - 1]->setColor();
					deleteBulletOf(enemies[e], k);
				}
			}
		}
	}

	//Loop for death animations.
	for (int d = 0; d < death.size(); d++)
	{
		if (death[d]->isOver())
			deleteMob(&death, death[d], d);
		else
			death[d]->update(*window);
	}
}

/*
 * Method made to learn how to use templates
 * and to save like 6 lines of code.
 * 
 * @param mobArray: vector of certain type.
 * @param mob: Mob to be deleted from vector.
 * @param index: Used to erase Mob from vector.
 */
template <typename T>
void Game::deleteMob(std::vector<T*>* mobArray, T* mob, int index)
{
	delete mob;
	mobArray->erase(mobArray->begin() + index);
}

/*
 * Simlar as above, but this one is for Mob Bullets.
 */
void Game::deleteBulletOf(Mob* mob, int index)
{
	delete mob->getBullets()[index];
	mob->getBullets().erase(mob->getBullets().begin() + index);
}

/*
 * Update stars. The way they work is
 * explained in Star class.
 */
void Game::updateStars()
{
	for (auto* s : stars)
		s->update(*window);
}

/*
 * Refreshes windows to next frame
 * and render all active objects.
 */
void Game::render()
{

	//if (state == GameState::Opening)
	//	window->clear(sf::Color(0, 0, 0, 255 - frame));

	window->clear(sf::Color(30, 30, 30));
	renderStars();
	renderMobs();

	//Debug
	if (debug)
	{
		renderDebug(*window);
		drawGrid();
	}




	ui->render(*window);

	if (state == GameState::Opening)
	{
		window->draw(blackscreen);
		blackscreen.setFillColor(sf::Color(0, 0, 0, 254 - (frame*2)));
		title.setColor(sf::Color(255, 255, 255, 128 + frame));
	}
	if(ui->getState() == UI::MenuState::Main)
		window->draw(title);



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

/*
 * Render stars here.
 */
void Game::renderStars()
{
	for (auto* s : stars)
		s->render(*window);
}


//-Debug

/* Update debug components */
void Game::updateDebug()
{
	std::stringstream str;

	str << "Score:" << score
		<< "\nX: " << player->getPos().x << " Y: " << player->getPos().y
		<< "\nBullets: " << player->activeBullets()
		<< "\nEnemies: " << enemies.size()
		<< "\nTileSize: " << tileSize
		<< "\nPlayer hp: " << player->getCurrentHP() << ", alive: " << player->isActive()
		<< "\nWidth (Global): " << player->bounds().width
		<< "\nHeight (Global): " << player->bounds().height
		<< "\nSway: " << player->getSway()
		<< "\nFrame: " << frame
		<< ((state == GameState::Opening) ? "\nState: Opening" :
			(state == GameState::MainMenu) ? "\nState: Main Menu" : 
			(state == GameState::Game) ? "\nState: Game" : "\nState: Pause Menu")
		<< ((ui->getState() == UI::MenuState::Main) ? "\nUI State: Main" :
			(ui->getState() == UI::MenuState::Controls) ? "\nUI State: Controls" :
			(ui->getState() == UI::MenuState::Options) ? "\nUI State: Options" :
			(ui->getState() == UI::MenuState::Pause) ? "\nUI State: Pause" : "\nUI State: Game");
	//Main, Controls, Options, Pause

	text.setString(str.str());


	///Trash code to check all enemies shooting per c frames.

		for (auto* e : enemies)
		{
			if(e->isActive())
				e->attackTo(player->getPos().x, player->getPos().y);
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
		linesVertical[k + 1].position = sf::Vector2f(tileSize * (k / 2), vidMode.height);
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
		linesHorizontal[k + 1].position = sf::Vector2f(vidMode.width, tileSize * (k / 2));
	}

	window->draw(linesVertical);
	window->draw(linesHorizontal);
}
