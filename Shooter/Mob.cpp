#include "Mob.h"

/*
 * I am putting this accessors for practice sake.
 * How tedious.
 * 
 * @return Mob's bullets.
 */
std::vector<Bullet*>& Mob::getBullets()
{
	return bullets;
}

/*
 * Get continum state of mob.
 * 
 * @return Bullet::Loop state.
 */
Bullet::Loop Mob::getContinum() const
{
	return continum;
}

/*
 * @return Limit of how many times can a Mob's bullet loop.
 */
sf::Vector2<short> Mob::getLoopLimit() const
{
	return loopLimit;
}

/* @return Remaining HP of mob. */
float Mob::getCurrentHP() const
{
	return hp;
}

/* @return Max HP of mob. */
short Mob::getMaxHP() const
{
	return maxHP;
}

/*
 * @return Number of active bullets.
 */
int Mob::activeBullets() const
{
	return bullets.size();
}
/*
 * @return shotRate of mob.
 */
short Mob::getShotRate() const
{
	return shotRate;
}

/*
 * Calculates how much damage the bullet makes.
 *
 * @param k: Index of Bullet in bullets vector.
 * @return Result of total damage dealt by bullet.
 */
float Mob::damageDealt(int k)
{
	return bullets[k]->getBaseDamage() * baseDamage;
}

/*
 * Attacks if cooldown is false, then makes it true.
 * If cooldown is true, counter increases.
 * When counter reaches shotRate, player can attack again.
 */
bool Mob::canAttack()
{
	if (bullets.size() > maxBullets)
		return false;

	if (cooldown)
	{
		if (cooldownCounter >= shotRate)
			cooldown = false;
		return false;
	}

	else
	{
		cooldown = true;
		cooldownCounter = 0;
		return true;
	}
}

/*
 * Sets Mob continum to certain state.
 * 
 * @param l: new Bullet::Loop state.
 */
void Mob::setContinum(Bullet::Loop l)
{
	continum = l;
}

/*
 * Sets shotRate to n.
 * 
 * @param n: new shotRate.
 */
void Mob::setShotRate(short n)
{
	shotRate = n;
}

/*
 * @param n: new value for LoopLimit.
 */
void Mob::setLoopLimit(sf::Vector2<short> n)
{
	loopLimit = n;
}

/*
 * Shoots bullet to specific direction.
 *
 * @param x: X coor of the target.
 * @param y: Y coor of the target.
 */
void Mob::attackTo(float destX, float destY)
{
	for (auto* b : bullets)
		if (b->getTarget().x == -1 && b->getTarget().y == -1)
			b->setAngle(destX, destY);;
}
