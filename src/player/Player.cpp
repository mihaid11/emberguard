#include "Player.h"
#include <iostream>
#include <cmath>

static float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

Player::Player(const sf::Vector2f& position, std::vector<Projectile>& projectiles)
    : mPosition(position), mTarget(nullptr), mFireRate(1.25f), mTimeSinceLastShot(0.0f), mRange(40.0f),
    mProjectiles(projectiles), mIsMoving(false)
{
    mShape.setRadius(10);
    mShape.setFillColor(sf::Color::Cyan);
    mShape.setPosition(position);

    mSpeed = 120.0f;
    mDamage = 10.0f;
    mHealth = 100.0f;
}

void Player::update(float dt, const std::vector<Enemy>& enemies, const sf::RectangleShape& topBorder, const sf::RectangleShape& bottomBorder,
    const sf::RectangleShape& leftBorder, const sf::RectangleShape& rightBorder)
{
    mIsMoving = false;
    bool canMoveLeftRight = true;
    bool canMoveUpDown = true; 
    sf::Vector2f originalPosition = mPosition;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        mPosition.y -= mSpeed * dt;
        mIsMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        mPosition.y += mSpeed * dt;
        mIsMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        mPosition.x -= mSpeed * dt;
        mIsMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        mPosition.x += mSpeed * dt;
        mIsMoving = true;
    }

    if (topBorder.getGlobalBounds().intersects(sf::FloatRect(mPosition.x, mPosition.y, mShape.getRadius() * 2.f, mShape.getRadius() * 2.f))) {
        canMoveUpDown = false;
    }
    if (bottomBorder.getGlobalBounds().intersects(sf::FloatRect(mPosition.x, mPosition.y, mShape.getRadius() * 2.f, mShape.getRadius() * 2.f))) {
        canMoveUpDown = false;
    }
    if (leftBorder.getGlobalBounds().intersects(sf::FloatRect(mPosition.x, mPosition.y, mShape.getRadius() * 2.f, mShape.getRadius() * 2.f))) {
        canMoveLeftRight = false;
    }
    if (rightBorder.getGlobalBounds().intersects(sf::FloatRect(mPosition.x, mPosition.y, mShape.getRadius() * 2.f, mShape.getRadius() * 2.f))) {
        canMoveLeftRight = false;
    }

    if (!canMoveUpDown) {
        mPosition.y = originalPosition.y;
    }
    if (!canMoveLeftRight) {
        mPosition.x = originalPosition.x;
    }

    mShape.setPosition(mPosition);

    // If the player is not moving, check for enemies and fire projectiles
    if (!mIsMoving) {
        mTimeSinceLastShot += dt;

        if (mTimeSinceLastShot >= mFireRate) {
            Enemy* selectedEnemy = nullptr;
            float closestDistance = mRange;

            for (const auto& enemy : enemies) {
                float dist = distance(mShape.getPosition(), enemy.getPosition());
                if (dist <= mRange && (selectedEnemy == nullptr || enemy.getSpawnTime() < selectedEnemy->getSpawnTime())) {
                    selectedEnemy = const_cast<Enemy*>(&enemy);
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
}

void Player::render(sf::RenderWindow& window)
{
    window.draw(mShape);
}

sf::Vector2f Player::getPosition()
{
    return mShape.getPosition();
}

bool Player::isInRange(const Enemy& enemy) const
{
    float range = mRange;
    float distance = std::sqrt(std::pow(mPosition.x - enemy.getPosition().x, 2.f) +
        std::pow(mPosition.y - enemy.getPosition().y, 2.f));
    return distance <= range;
}

float Player::getAttackRange() const
{
    return mRange;
}

float Player::getPlacementRange() const
{
    return PlacementRange;
}

sf::Vector2f Player::getPosition() const
{
    return mShape.getPosition();
}

float Player::getDamage() const
{
    return mDamage;
}

float Player::getFireRate() const
{
    return mFireRate;
}

sf::FloatRect Player::getBounds() const
{
    return mShape.getGlobalBounds();
}

void Player::setPosition(const sf::Vector2f& position)
{
    mShape.setPosition(position);
    mPosition = position;
}

void Player::fireProjectile()
{
    if (!mTarget) return;

    sf::Vector2f targetPos = mTarget->getPosition();
    sf::Vector2f direction = targetPos - mShape.getPosition();
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= magnitude;

    sf::Vector2f projectilePos = mShape.getPosition();
    mProjectiles.emplace_back(projectilePos, mTarget, 200.0f, 5.0f);
}

void Player::takeDamage(float amount)
{
    mHealth -= amount;
    if (mHealth < 0)
        mHealth = 0;
}

float Player::getHealth() const
{
    return mHealth;
}

void Player::setHealth(float health)
{
    mHealth = health;
}
