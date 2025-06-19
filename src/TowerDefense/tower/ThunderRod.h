#pragma once
#include "Tower.h"
#include <SFML/Graphics.hpp>
#include "../enemy/Enemy.h"
#include "../projectiles/FlameProjectile.h"
// #include "../projectilehtningProjectile.h"

class ThunderRod : public Tower {
public:
    ThunderRod(const sf::Vector2f& position, std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies);

private:
    int mMaxChains;
    float mChainRange;
    std::vector<Enemy>& mEnemies;
};

