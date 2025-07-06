#include "ThunderRod.h"
#include <math.h>

static float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

ThunderRod::ThunderRod(const sf::Vector2f& position, std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies)
    : Tower(position, projectiles), mMaxChains(3), mChainRange(100.f), mEnemies(enemies),
      mFireCooldown(1.25f), mProjectileSpeed(200.0f), mPosition(position), mDamage(5) {
    mShape.setFillColor(sf::Color::Cyan);

    mRange = 120.0f;
    mFireRate = 2.25f;
    mDamage = 14.0f;
    mCost = 200;

    // mUpgrades.clear();
    // mUpgrades.push_back(Upgrade("Chain Damage", 200, 5, [this]() { mDamage += 5; }));
    // mUpgrades.push_back(Upgrade("Chain Range", 150, 10, [this]() { mChainRange += 15.0f; }));
    // mUpgrades.push_back(Upgrade("Chain Count", 300, 1, [this]() { mMaxChains++; }));
}

void ThunderRod::update(float dt, std::vector<Enemy>& enemies)
{
    mFireCooldown -= dt;
    if (mFireCooldown <= 0.0f) {
        Enemy* closestEnemy = nullptr;
        float closestDistance = mRange;

        for (auto& enemy : enemies) {
            float distance = std::hypot(enemy.getPosition().x - mPosition.x, enemy.getPosition().y - mPosition.y);
            if (distance < closestDistance) {
                closestEnemy = &enemy;
                closestDistance = distance;
            }
        }

        if (closestEnemy != nullptr) {
            mProjectiles.emplace_back(LightingChain(mPosition, closestEnemy, mProjectileSpeed, mDamage));
            mFireCooldown = mFireRate;
        }
    }

    for (auto& projectile : mProjectiles)
        projectile.update(dt);
}
