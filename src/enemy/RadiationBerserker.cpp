#include "RadiationBerserker.h"

RadiationBerserker::RadiationBerserker(const std::vector<sf::Vector2f>& path)
	: Enemy(path)
{
	mShape.setRadius(10);
	mShape.setFillColor(sf::Color(200, 100, 50, 250));
	mSpeed = 31.0f;
	mHealth = 32.0f;
	mDamageToPlayer = 15.0f;
	mDamageToGame = 15.0f;
	mAttackSpeed = 2.0f;
	mAttackRange = 40.0f;
	mFollowRange = 70.0f;
	mSpawnTime = 2.7f;
	mReward = 50;
}
