#pragma once
#include "Tower.h"
#include "../projectiles/Projectile.h"
#include <vector>
#include <SFML/Graphics.hpp>

class LaserTower : public Tower
{
public:
	LaserTower(const sf::Vector2f& position, std::vector<Projectile>& projectiles);

	void update(float dt, std::vector<Enemy>& enemies) override;

private:
	void applyInitialUpgrade();
	void applyHighDamageUpgrade();
	void applyContinuousUpgrade();

	float mFireCooldown;
	float mProjectileSpeed;
	sf::Vector2f mPosition;
	float mDamage;
	bool mIsHighDamage;
	bool mIsContinuous;

	std::vector<Upgrade> mUpgrades;
};

