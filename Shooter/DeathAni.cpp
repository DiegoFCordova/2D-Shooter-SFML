#include "DeathAni.h"

/*
 * Initiates basic variables:
 * All sprites that will be used for the animation, sets
 * first texture as sprite, frames to 0, and done to false.
 */
void DeathAni::init()
{
	aniSpeed = 2;
	currentFrame = 0;

	std::stringstream path;

	switch (type)
	{
	case DeathAni::Type::Enemy:
		path << "Sprites/Death";
		loadTextures(4, path);
		break;

	case DeathAni::Type::Bullet:
		path << "Sprites/BulletImpact";
		loadTextures(4, path);
		break;
	
	case DeathAni::Type::Player:
		aniSpeed = 6;
		path << "Sprites/ShipDeath";
		loadTextures(6, path);
		break;
	
	default:
		std::cout << "Should never get to here on DeathAni's init\n";
		break;
	}

	sprite.setTexture(*textures[0]);
	frames = 0;
	done = false;
}

/*
 * Sets bullet sprite to certain color.
 * If not arguments present, sets it red for enemy.
 *
 * @param r: Red.
 * @param g: Green.
 * @param b: Blue.
 * @param a: Alpha.
 */
void DeathAni::setColor(float r, float g, float b, float a)
{
	if (r == -1)
		sprite.setColor(sf::Color(255, 50, 54, 150));

	else
		sprite.setColor(sf::Color(r, g, b, a));	//Nice color: 0, 100, 154, 100
}

/*
 * Method to get explosion scale to the biggest side of 
 * given object.
 * This method assumes that all sprites original
 * resolution is the same for both width and height.
 * 
 * @param side: used for calculation of scale.
 * @return scale to set sprite to.
 */
float DeathAni::getFitScale(float side) const
{
	return side / sprite.getLocalBounds().height;
}

/*
 * Creates n number of textures to load them up.
 * 
 * @param n: Number of frames for animation.
 * @param str: Beginning of the path for loading animation.
 */
void DeathAni::loadTextures(int n, const std::stringstream& str)
{
	std::stringstream result;
	for (int k = 0; k < n; k++)
	{
		textures.emplace_back(new sf::Texture());
		result << str.str() << k << ".png";

		if (!textures[k]->loadFromFile(result.str()))
			std::cout << "Error loading " << result.str() << ".\n";

		result.str(std::string());
	}
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
 * @param side: used for getFitScale.
 * @param t: Will decide which textures are loaded.
 */
DeathAni::DeathAni(float x, float y, float side, Type t)
{
	type = t;
	init();
	scale = getFitScale(side) * 1.2f;
	sprite.setOrigin(sprite.getPosition().x + (sprite.getLocalBounds().width / 2), sprite.getPosition().y + (sprite.getLocalBounds().height / 2));
	sprite.setScale(scale, scale);
	sprite.setPosition(x, y);
}

/* Use if using pointers. */
DeathAni::~DeathAni()
{
	for (auto* t : textures)
		delete t;
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
	if (frames > aniSpeed * (currentFrame + 1))
	{
		currentFrame++;
		if (currentFrame == textures.size())
			done = true;
		else
			sprite.setTexture(*textures[currentFrame]);
	}

	frames = frames + (1 * DT::dt * DT::mult);
}

void DeathAni::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
