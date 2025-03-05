#include "EmpireScout.h"

EmpireScout::EmpireScout(const std::vector<sf::Vector2f>& path) : Enemy(path)
{
	mShape.setRadius(10);
	mShape.setFillColor(sf::Color(100, 150, 200, 250));
	mSpeed = 80.0f;
	mHealth = 10.0f;
	mDamageToPlayer = 8.0f;
	mDamageToGame = 8.0f;
	mAttackSpeed = 0.75f;
	mAttackRange = 30.0f;
	mFollowRange = 50.0f;
	mSpawnTime = 0.60f;
	mReward = 10;
}
