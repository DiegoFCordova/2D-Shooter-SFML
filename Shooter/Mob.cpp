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
 * Calculates how much damage the bullet makes.
 *
 * @param k: Index of Bullet in bullets vector.
 * @return Result of total damage dealt by bullet.
 */
float Mob::damageDealt(int k)
{
	return bullets[k]->getBaseDamage();		///Add damage multiplier later
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
 * Shoots bullet to specific direction.
 * ///Needs a lot of tuning
 *
 * @param x: X coor of the target.
 * @param y: Y coor of the target.
 */
void Mob::attackTo(float destX, float destY)
{
	//if maxBullets
	bullets.emplace_back(new Bullet(sprite.getPosition().x, sprite.getPosition().y + sprite.getGlobalBounds().height/2, 0));
	///bullets.emplace_back(new Bullet(srcX, srcY, 0));

	for (auto* b : bullets)
		if (b->getTarget().x == -1 && b->getTarget().y == -1)
			b->setAngle(destX, destY);

	//			sprite.move(sway, -velocity);
	//if (canAttack())
//		bullets.emplace_back(new Bullet(sprite.getPosition().x + (sprite.getGlobalBounds().width / 2), sprite.getPosition().y, sway));
}
