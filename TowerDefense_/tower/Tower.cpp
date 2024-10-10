#include "Tower.h"
#include <iostream>
#include <cmath>
#include <stdexcept>

static float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

Tower::Tower(const sf::Vector2f& position, std::vector<Projectile>& projectiles)
    : mRange(120.0f), mFireRate(1.25f), mTimeSinceLastShot(0.0f), mTarget(nullptr),
    mSelected(false), mProjectiles(projectiles), mCost(100), mDamage(5) {
    mShape.setRadius(15);
    mShape.setFillColor(sf::Color::Green);
    mShape.setPosition(position);

    // Define upgrades
    mUpgrades.push_back(Upgrade("Damage Upgrade", 200, 5, [this]() { mDamage += 5; }));
    mUpgrades.push_back(Upgrade("Range Upgrade", 150, 10, [this]() { mRange += 10.0f; }));
    mUpgrades.push_back(Upgrade("Fire Rate Upgrade", 100, mFireRate * 0.05f, [this]() { mFireRate *= 0.95f; }));
}

void Tower::update(float dt, std::vector<Enemy>& enemies) {
    mTimeSinceLastShot += dt;

    if (mTimeSinceLastShot >= mFireRate) {
        Enemy* selectedEnemy = nullptr;
        float closestDistance = mRange;

        for (auto& enemy : enemies) {
            float dist = distance(mShape.getPosition(), enemy.getPosition());
            if (dist <= mRange && (selectedEnemy == nullptr || enemy.getSpawnTime() < selectedEnemy->getSpawnTime())) {
                selectedEnemy = &enemy;
            }
        }

        mTarget = selectedEnemy;

        if (mTarget) {
            fireProjectile();
            mTimeSinceLastShot = 0.0f;
        }
    }

    if (!mTarget || mTarget->isDead() || distance(mShape.getPosition(), mTarget->getPosition()) > mRange) {
        mTarget = nullptr;
    }
}

void Tower::render(sf::RenderWindow& window)
{
    window.draw(mShape);
    for (auto& projectile : mProjectiles) {
        projectile.render(window);
    }
}

sf::Vector2f Tower::getPosition() const
{
    return mShape.getPosition();
}

int Tower::getCost() const {
    return mCost;
}

sf::Vector2f Tower::getSize() const
{
    return sf::Vector2f(mShape.getRadius(), mShape.getRadius());
}

float Tower::getDamage() const
{
    return mDamage;
}

float Tower::getFireRate() const
{
    return mFireRate;
}

int Tower::getSellPrice() const
{
    return (int)(mCost * 0.4f);
}


void Tower::setPosition(const sf::Vector2f& position) {
    mShape.setPosition(position);
}

Enemy* Tower::getTarget() const
{
    return mTarget;
}

bool Tower::containsPoint(const sf::Vector2f& point) const
{
    return mShape.getGlobalBounds().contains(point);
}

void Tower::setSelected(bool selected)
{
    mSelected = selected;
}

float Tower::getRange() const {
    return mRange;
}

void Tower::upgrade(size_t index, int& crystals) {
    if (canUpgrade(index)) {
        if (crystals >= mUpgrades[index].getCost()) {
            mUpgrades[index].apply();
            crystals -= mUpgrades[index].getCost();
        }
        else
            std::cout << "Not enought crystals for upgrade" << std::endl;
    }
    else
        std::cout << "No upgrade available" << std::endl;
}

void Tower::fireProjectile()
{
    if (!mTarget) return;

    sf::Vector2f targetPos = mTarget->getPosition();
    sf::Vector2f direction = targetPos - mShape.getPosition();
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= magnitude;

    sf::Vector2f projectilePos = mShape.getPosition();
    mProjectiles.emplace_back(projectilePos, mTarget, 200.0f, mDamage);

    std::cout << "Fired projectile from tower at " << projectilePos.x << ", " << projectilePos.y << std::endl;
}

bool Tower::canUpgrade(size_t index) const {
    return index < mUpgrades.size();
}

size_t Tower::getUpgradeCount() const {
    return mUpgrades.size();
}

void Tower::addUpgrade(const Upgrade& upgrade) {
    mUpgrades.push_back(upgrade);
}

const Upgrade& Tower::getUpgrade(size_t index) const {
    if (index < mUpgrades.size()) {
        return mUpgrades[index];
    }
    throw std::out_of_range("Upgrade index out of range");
}

int Tower::sell()
{
    int sellValue = static_cast<int>(mCost * 0.4f);
    std::cout << "Tower sold for " << sellValue << " crystals." << std::endl;
    return sellValue;
}
