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

	ticks = 0;
	openingFrame = 0;
	enemySpawnRate = 70;
	maxEnemies = 20;
	score = 0;
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

/*
 * Primarily used when going back to Main
 * Menu. Resets Player stats and removes all enemies.
 */
void Game::resetMobs()
{
	player->resetMob();
	player->setPos(sf::Vector2<float>(tileSize * 16, tileSize * 15));
	score = 0;
	refreshUI();
	enemies.clear();
}

/*
 * Refreshes UI parts that interact with Player.
 */
void Game::refreshUI()
{
	ui->optionSet((int)ui->getDifficulty(), 0);
	ui->optionSet(player->getShotRate(), 1);
	ui->optionSet(player->getShotSpeed(), 2);
	ui->optionSet(player->getVelocity(), 3);
	ui->optionSet((int)player->getContinum(), 4);
	ui->optionSet(player->getLoopLimit().y, 5);
	ui->optionSet(enemySpawnRate, 6);
	ui->optionSet(player->getLives(), 7);

	ui->updateGameScores(0, score);
	ui->updateGameScores(1, player->getCurrentHP());
	ui->updateGameScores(2, player->getLives());
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

	//Debug texts
	text.setFont(font);
	text.setCharacterSize(tileSize*.85);
	text.setFillColor(sf::Color(218, 218, 218, 250));
	text.setString("TEXT GOES HERE");

	//Temp text
	temp.setFont(font);
	temp.setCharacterSize(tileSize);
	temp.setPosition(vidMode.width/2 - (tileSize*3.25), vidMode.height/2 - tileSize);
	temp.setFillColor(sf::Color(255, 255, 255, 255));
	temp.setString("X Lives left");


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

	refreshUI();
}


/*
 * Constructor. Calls init methods, described above.
 */
Game::Game()
{
	initWindow();
	initVars();
	initMobs();
	initText();
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
	ticks++;

	if(state == GameState::Game)
		updateTimer();

	pollEvents();

	if (state == GameState::Opening)
	{
		openingFrame++;
		if (openingFrame == 127)
			state = GameState::MainMenu;
	}

	if(state == GameState::Game)
		updateMobs();

	updateStars();

	//-Debug
	updateDebug();
}

/*
 * Updates frames to keep track of logic,
 * events, and revive sequence.
 */
void Game::updateTimer()
{
	if (player->getStatus() == Player::Status::Dead)
	{
		reviveTicks++;
		if (reviveTicks == 100)
		{
			if (player->getLives() == 0)
			{
				temp.move(tileSize*.85, 0);
				temp.setString("Game Over");
				reviveTicks = 0;
				state = GameState::GameOver;
				resetMobs();
				///change gameMode to GameOver (Hide all mobs)
				///Return player to Main Menu after 100 ticks or so
			}

			/// Add text for duration of wait (Text will depend if infinite lifes or not)
			///		If no lifes left, change text at last few frames saying GAME OVER
			///		(Maybe add HighScore file)
			/// 
			else
			{
				player->revive();
				player->setPos(sf::Vector2f(tileSize * 16, tileSize * 15));
				reviveTicks = 0;
				ui->updateGameScores(1, player->getCurrentHP());
			}
		}
	}

	//Enemy auto-shoot.
	for (auto* e : enemies)
	{
		if (e->isActive() && player->isActive())
			e->attackTo(player->getPos().x, player->getPos().y);
	}

	if(ticks % enemySpawnRate == 0 && enemies.size() < maxEnemies && player->isActive())
		enemies.emplace_back(new Enemy(rand() % vidMode.width, rand() % (vidMode.height / 2), ui->getDifficulty()));
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
			{
				if (!skipOpening())
					ui->up();
			}
			else if (ev.key.code == sf::Keyboard::S || ev.key.code == sf::Keyboard::Down)
			{
				if (!skipOpening())
					ui->down();
			}

			else if (ev.key.code == sf::Keyboard::D || ev.key.code == sf::Keyboard::Right)
			{
				if (ui->getState() == UI::MenuState::Options)
				{
					switch (ui->getChoice())
					{
					case 0:
						ui->optionSet((int)ui->getDifficulty() + 1);
						break;
					case 1:
						player->setShotRate(ui->optionSet(player->getShotRate() + 1));
						break;
					case 2:
						player->setShotSpeed(ui->optionSet(player->getShotSpeed() + 1));
						break;
					case 3:
						player->setVelocity(ui->optionSet(player->getVelocity() + 1));
						break;
					case 4:
						ui->optionSet((int)player->getContinum() + 1);
						switch (player->getContinum())
						{
						case Bullet::Loop::None:
							player->setContinum((Bullet::Loop)((int)player->getContinum()+1));// Bullet::Loop::Horizontal);
							break;
						case Bullet::Loop::Horizontal:
							player->setContinum(Bullet::Loop::Vertical);
							break;
						case Bullet::Loop::Vertical:
							player->setContinum(Bullet::Loop::All);
							break;
						case Bullet::Loop::All:
							player->setContinum(Bullet::Loop::None);
							break;
						default:
							break;
						}
						break;
					case 5:
						player->setLoopLimit(sf::Vector2<short>(-1, ui->optionSet(player->getLoopLimit().y + 1)));
						break;
					case 6:
						enemySpawnRate = ui->optionSet(enemySpawnRate + 1);
						break;
					case 7:
						player->setLives(ui->optionSet(player->getLives() + 1));
						ui->updateGameScores(2, player->getLives());
						break;
					default:
						break;
					}
				}
			}
			else if (ev.key.code == sf::Keyboard::A || ev.key.code == sf::Keyboard::Left)
			{
				if (ui->getState() == UI::MenuState::Options)
				{
					switch (ui->getChoice())
					{
					case 0:
						ui->optionSet((int)ui->getDifficulty() - 1);
						break;
					case 1:
						player->setShotRate(ui->optionSet(player->getShotRate() - 1));
						break;
					case 2:
						player->setShotSpeed(ui->optionSet(player->getShotSpeed() - 1));
						break;
					case 3:
						player->setVelocity(ui->optionSet(player->getVelocity() - 1));
						break;
					case 4:
						ui->optionSet((int)player->getContinum() - 1);
						switch (player->getContinum())
						{
						case Bullet::Loop::None:
							player->setContinum(Bullet::Loop::All);
							break;
						case Bullet::Loop::Horizontal:
							player->setContinum(Bullet::Loop::None);
							break;
						case Bullet::Loop::Vertical:
							player->setContinum(Bullet::Loop::Horizontal);
							break;
						case Bullet::Loop::All:
							player->setContinum(Bullet::Loop::Vertical);
							break;
						default:
							break;
						}
						break;
					case 5:
						player->setLoopLimit(sf::Vector2<short>(-1, ui->optionSet(player->getLoopLimit().y - 1)));
						break;
					case 6:
						enemySpawnRate = ui->optionSet(enemySpawnRate - 1);
						break;
					case 7:
						player->setLives(ui->optionSet(player->getLives() - 1));
						ui->updateGameScores(2, player->getLives());
						break;
					default:
						break;
					}
				}
			}
			else if (ev.key.code == sf::Keyboard::Enter)
			{
				if (!skipOpening() && (ui->getState() == UI::MenuState::Main || ui->getState() == UI::MenuState::Pause))
				{
					switch (ui->enter())
					{
					case 0:
						state = GameState::Game;
						break;
					case 2:
						state = GameState::MainMenu;
						resetMobs();
						break;
					case -1:
						window->close();
					default:
						break;
					}
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
				openingFrame = 0;
				state = GameState::Opening;
			}
			else if (ev.key.code == sf::Keyboard::Q)
				enemies.emplace_back(new Enemy(rand() % vidMode.width, rand() % (vidMode.height/2), ui->getDifficulty()));
			else if (ev.key.code == sf::Keyboard::E)
				enemies.emplace_back(new Enemy(vidMode.width / 2, vidMode.height / 2, ui->getDifficulty()));
			else if (ev.key.code == sf::Keyboard::R)
				death.emplace_back(new DeathAni(rand() % vidMode.width, rand() % vidMode.height, 10));
			else if (ev.key.code == sf::Keyboard::L)
				resetMobs();
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
	if (player->isActive())
		player->updateInput();

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
						score = ui->updateGameScores(0, score + 5);
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
					if (player->getStatus() == Player::Status::Alive)
					{
						player->takeDamage(enemies[e]->damageDealt(k));
						ui->updateGameScores(1, player->getCurrentHP());
					}

					if (player->getStatus() == Player::Status::Dead)
					{
						std::stringstream ss;
						if (player->getLives() == 0)
						{
							ss << "Out of Lives";
						}
						else if (player->getLives() == 1)
							ss << "Last Chance";
						else
							ss << player->getLives() << " Lives Left";

						temp.setString(ss.str());
						ui->optionSet(player->getLives(), 7);
						ui->updateGameScores(2, player->getLives());

						death.emplace_back(new DeathAni(player->getPos().x, player->getPos().y,
											player->getLargestSide(), DeathAni::Type::Player));
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
 * Quick method to save lines.
 * Returns true if state is Opening
 * and skips it.
 * Else, return false.
 */
bool Game::skipOpening()
{
	if (state == GameState::Opening)
	{
		openingFrame = 127;
		title.setColor(sf::Color(255, 255, 255, 255));
		state = GameState::MainMenu;
		return true;
	}

	return false;
}

/*
 * Update stars. Also makes changes
 * when updating difficulty.
 */
void Game::updateStars()
{
	if (ui->getDifficulty() == UI::Difficulty::Merciless && stars[0]->getFX() != Star::FX::Masa)
	{
		for (auto* s : stars)
			s->masaFX();
		enemySpawnRate = 45;
	}
	else if (ui->getDifficulty() == UI::Difficulty::Normal && 
				stars[0]->getFX() != Star::FX::Normal && stars[0]->isReadyToChange())
	{
		for (auto* s : stars)
			s->normalFX();
	}
	else if (ui->getDifficulty() == UI::Difficulty::Easy && 
				stars[0]->getFX() != Star::FX::InverseSpeed && stars[0]->isReadyToChange())
	{
		for (auto* s : stars)
			s->inverseSpeedFX();
		enemySpawnRate = 150;
	}

		for (auto* s : stars)
		{
			if (player->isActive())
				s->update(*window);
			s->blinkFX(ticks);
		}
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



	if (state == GameState::Game || state == GameState::PauseMenu)
	{
		renderMobs();
		if(player->getStatus() == Player::Status::Dead || state == GameState::GameOver)
			window->draw(temp);
	}

	if(state != GameState::GameOver)
		ui->render(*window);

	if (state == GameState::Opening)
	{
		window->draw(blackscreen);
		blackscreen.setFillColor(sf::Color(0, 0, 0, 254 - (openingFrame *2)));
		title.setColor(sf::Color(255, 255, 255, 128 + openingFrame));
	}

	if(ui->getState() == UI::MenuState::Main)
		window->draw(title);

	//Debug
	if (debug)
		renderDebug(*window);

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

	str << "\Ticks: " << ticks
		<< "\nX: " << player->getPos().x << " Y: " << player->getPos().y
		<< "\nBullets: " << player->activeBullets()
		<< "\nEnemies: " << enemies.size()
		<< "\nTileSize: " << tileSize
		<< "\nPlayer hp: " << player->getCurrentHP() << ", alive: " << player->isActive()
		<< "\nPlayer lives left: " << player->getLives()
		<< "\nRevive Ticks: " << reviveTicks
		<< "\nSway: " << player->getSway()
		<< "\nOpeningFrame: " << openingFrame
		<< ((state == GameState::Opening) ? "\nState: Opening" :
			(state == GameState::MainMenu) ? "\nState: Main Menu" : 
			(state == GameState::Game) ? "\nState: Game" :
			(state == GameState::GameOver) ? "\nState: GameOver" : "\nState: Pause Menu")
		<< ((ui->getState() == UI::MenuState::Main) ? "\nUI State: Main" :
			(ui->getState() == UI::MenuState::Controls) ? "\nUI State: Controls" :
			(ui->getState() == UI::MenuState::Options) ? "\nUI State: Options" :
			(ui->getState() == UI::MenuState::Pause) ? "\nUI State: Pause" : "\nUI State: Game")
		<< ((ui->getDifficulty() == UI::Difficulty::Easy) ? "\nDifficulty: Easy" :
			(ui->getDifficulty() == UI::Difficulty::Normal) ? "\nDifficulty: Normal" : "\nDifficulty: Merciless")
		<< ((stars[0]->getFX() == Star::FX::Normal) ? "\nStar Pattern: Normal" : 
			(stars[0]->getFX() == Star::FX::InverseSpeed) ? "\nStar Pattern: Invert" : "\nStar Pattern: Masa");
	//Main, Controls, Options, Pause

	text.setString(str.str());
}

/* Render debug components after update is done */
void Game::renderDebug(sf::RenderTarget& target)
{
	target.draw(text);
	drawGrid();
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
