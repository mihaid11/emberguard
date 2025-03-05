#include "FlameTurret.h"

static float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

FlameTurret::FlameTurret(const sf::Vector2f& position, std::vector<Projectile>& projectiles)
    : Tower(position, projectiles) {
    mRange = 100.0f;
    mFireRate = 2.0f;
    mDamage = 10.0f;
    mCost = 150;
    mShape.setFillColor(sf::Color(255, 84, 84, 255));
    //mUpgrades.push_back(Upgrade("Increase Burn Duration", 200, 5, [this]() { /* Increase burn duration logic */ }));
    //mUpgrades.push_back(Upgrade("Wider Flame Arc", 300, 10, [this]() { /* Increase flame arc width */ }));
    //mUpgrades.push_back(Upgrade("Faster Fire Rate", 250, mFireRate * 0.10f, [this]() { mFireRate *= 0.9f; }));
}

void FlameTurret::update(float dt, std::vector<Enemy>& enemies) {
    mTimeSinceLastShot += dt;

    if (mTimeSinceLastShot >= mFireRate) {
        Enemy* selectedEnemy = nullptr;
        float closestDistance = mRange;

        for (auto& enemy : enemies) {
            float dist = distance(mShape.getPosition(), enemy.getPosition());
            if (dist <= mRange) {
                selectedEnemy = &enemy;
                fireFlame();
                mTimeSinceLastShot = 0.0f;
                break;
            }
        }

        mTarget = selectedEnemy;
    }

    if (!mTarget || mTarget->isDead() || distance(mShape.getPosition(), mTarget->getPosition()) > mRange) {
        mTarget = nullptr;
    }
}

int FlameTurret::getType() const
{
    return 1;
}

void FlameTurret::fireFlame() {
    if (!mTarget) return;

    // Fire the flame projectile
    sf::Vector2f projectilePos = mShape.getPosition();
    mProjectiles.emplace_back(FlameProjectile(projectilePos, mTarget, 100.0f, mDamage));
}
