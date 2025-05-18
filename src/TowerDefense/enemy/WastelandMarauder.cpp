#include "WastelandMarauder.h"

WastelandMarauder::WastelandMarauder(const std::vector<sf::Vector2f>& path)
	: Enemy(path)
{
	mShape.setRadius(10);
	mShape.setFillColor(sf::Color::Yellow);
	mSpeed = 50.0f;
	mHealth = 10.0f;
	mDamageToPlayer = 5.0f;
	mDamageToGame = 10.0f;
	mAttackSpeed = 1.0f;
	mAttackRange = 50.0f;
	mFollowRange = 100.0f;
	mSpawnTime = 1.7f;
	mReward = 25;
}