#include "LaserProjectile.h"
#include <iostream>
#include <math.h>

LaserProjectile::LaserProjectile(const sf::Vector2f& position, Enemy* target, float speed, float damage)
	: Projectile(position, target, speed, damage)
{
	mLaserLine.setPrimitiveType(sf::Lines);
	mLaserLine.resize(2);

	mLaserLine[0].position = position;
	mLaserLine[0].color = sf::Color::Red;

	mLaserLine[1].position = target->getPosition();
	mLaserLine[1].color = sf::Color::Red;
}

void LaserProjectile::update(float dt)
{
	if (mTarget == nullptr || mTarget->isDead()) {
		mHitTarget = true;
		return;
	}

	mLaserLine[0].position = getPosition();
	mLaserLine[1].position = mTarget->getPosition();

	if (checkCollision(mTarget)) {
		mTarget->takeDamage(getDamage());
		mHitTarget = true;
	}
}

void LaserProjectile::render(sf::RenderWindow& window)
{
	if (!mHitTarget)
		window.draw(mLaserLine);
}
