#pragma once
#include "Tower.h"
#include <SFML/Graphics.hpp>
#include "../enemy/Enemy.h"
#include "../projectiles/LightingChain.h"
// #include "../projectilehtningProjectile.h"

class ThunderRod : public Tower {
public:
    ThunderRod(const sf::Vector2f& position, std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies);

    void update(float dt, std::vector<Enemy>& enemies) override;
private:
    int mMaxChains;
    float mChainRange;
    std::vector<Enemy>& mEnemies;

    float mFireCooldown;
    float mProjectileSpeed;
    sf::Vector2f mPosition;
    float mDamage;
    bool mIsHighDamage;
    bool mIsContinuous;
};

