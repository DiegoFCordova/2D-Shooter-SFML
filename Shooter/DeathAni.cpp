#include "DeathAni.h"

/*
 * Initiates basic variables.
 */
void DeathAni::init()
{
	if (!tex1.loadFromFile("Sprites/Death1.png"))
		std::cout << "Error loading DeathAni Sprite 1.\n";

	if (!tex2.loadFromFile("Sprites/Death2.png"))
		std::cout << "Error loading DeathAni Sprite 2.\n";

	if (!tex3.loadFromFile("Sprites/Death3.png"))
		std::cout << "Error loading DeathAni Sprite 3.\n";

	if (!tex4.loadFromFile("Sprites/Death4.png"))
		std::cout << "Error loading DeathAni Sprite 4.\n"; 
	
	sprite.setTexture(tex1);
	sprite.scale(2, 2);
	frames = 0;
	done = false;
}


/*
 * Most likely never used, but here just in case.
 */
DeathAni::DeathAni()
{
	init();
}

/*
 * Constuctor
 */
DeathAni::DeathAni(float x, float y)
{
	init();
	sprite.setPosition(x, y);
}

/*
 * @return true if animation is over.
 */
bool DeathAni::isOver() const
{
	return done;
}


void DeathAni::update(sf::RenderTarget& target)
{
	if (frames > 20)
		done = true;
	else if (frames > 15)
		sprite.setTexture(tex4);
	else if (frames > 10)
		sprite.setTexture(tex3);
	else if (frames > 5)
		sprite.setTexture(tex2);

	frames++;
}

void DeathAni::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
