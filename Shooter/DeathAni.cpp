#include "DeathAni.h"

/*
 * Initiates basic variables:
 * All sprites that will be used for the animation, sets
 * first texture as sprite, frames to 0, and done to false.
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
	frames = 0;
	done = false;
}

/*
 * Method to get explosion scale to the biggest side of 
 * given object.
 * This method assumes that all sprites original
 * resolution is the same for both width and height.
 * 
 * @param width: Width of give object (Global).
 * @param height: Height of given object (Global).
 * @return scale to set sprite to.
 */
float DeathAni::getFitScale(float side) const
{
	return side / sprite.getLocalBounds().height;
}


/*
 * Should never used, but here just in case.
 */
DeathAni::DeathAni()
{
	init();
}

/*
 * Constuctor. Sets sprite origin to center,
 * Scale depending on third and fourth variable.
 * 
 * @param x: X position.
 * @param y: Y position.
 * @param width: Some object width.
 * @param height: Some object height.
 */
DeathAni::DeathAni(float x, float y, float side)
{
	init();
	scale = getFitScale(side) * 1.2;
	sprite.setOrigin(sprite.getPosition().x + (sprite.getLocalBounds().width / 2), sprite.getPosition().y + (sprite.getLocalBounds().height / 2));
	sprite.setScale(scale, scale);
	sprite.setPosition(x, y);
}

/* Use if using pointers. */
DeathAni::~DeathAni()
{

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
