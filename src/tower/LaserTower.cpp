#include "LaserTower.h"
#include "../projectiles/LaserProjectile.h"
#include <iostream>
#include <cmath>

LaserTower::LaserTower(const sf::Vector2f& position, std::vector<Projectile>& projectiles)
    : Tower(position, projectiles), mFireCooldown(1.25f), mProjectileSpeed(200.0f), mPosition(position), mDamage(5),
    mIsContinuous(false), mIsHighDamage(false)
{
    mCost = 100;
}

void LaserTower::applyInitialUpgrade() {
    mDamage += 5;
    mFireCooldown /= 1.3f;
    mShape.setFillColor(sf::Color::Yellow);
}

void LaserTower::applyHighDamageUpgrade() {
    mIsHighDamage = true;
    mIsContinuous = false;
    mDamage *= 2;
    mFireCooldown /= 2;
    mShape.setFillColor(sf::Color::Red);
}

void LaserTower::applyContinuousUpgrade() {
    mIsHighDamage = false;
    mIsContinuous = true;
    mDamage /= 2;
    mShape.setFillColor(sf::Color::Green);
}

void LaserTower::update(float dt, std::vector<Enemy>& enemies)
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
            mProjectiles.emplace_back(LaserProjectile(mPosition, closestEnemy, mProjectileSpeed, mDamage));
            mFireCooldown = mFireRate;
        }
    }

    for (auto& projectile : mProjectiles) {
        projectile.update(dt);
    }
}

int LaserTower::getType() const
{
    return 0;
}
