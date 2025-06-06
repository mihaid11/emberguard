#include "Enemy.h"
#include "../player/Player.h"
#include <cmath>
#include <iostream>

Enemy::Enemy(const std::vector<sf::Vector2f>& path)
    : mPath(path), mCurrentWaypoint(0), mTimeSinceLastAttack(0.0f)
{
    mShape.setRadius(10);
    mShape.setFillColor(sf::Color::Blue);
    if (!mPath.empty())
        mShape.setPosition(mPath[0]);

    // Enemy health bar
    mHealthBar.setSize(sf::Vector2f(40, 5));
    mHealthBar.setFillColor(sf::Color::Red);
    if (!mPath.empty())
        mHealthBar.setPosition(mShape.getPosition().x + 10, mShape.getPosition().y - 10);
    else
        std::cout << "Enemy created with empty path!" << std::endl;

    mSpawnTime = 1.7f;
    mSpeed = 50.0f;
    mHealth = 10.0f;
    mDamageToPlayer = 5.0f;
    mDamageToGame = 10.0f;
    mAttackSpeed = 1.0f;
    mAttackRange = 50.0f;
    mFollowRange = 100.0f;
    mReward = 25;

    mTimeSinceLastBurn = 0.0f;
    mBurnDuration = 0.0f;
    mBurnDamage = 0.0f;
}

void Enemy::update(float dt, Player& player)
{
    float playerDistance = std::sqrt(std::pow(mShape.getPosition().x - player.getPosition().x, 2.f) +
        std::pow(mShape.getPosition().y - player.getPosition().y, 2.f));

    if (playerDistance <= mFollowRange)
    {
        if (playerDistance <= mAttackRange) {
            attackPlayer(dt, player);
        }
        else {
            followPlayer(dt, player);
        }
    }
    else
    {
        followPath(dt);
    }

    mHealthBar.setPosition(mShape.getPosition().x + 10, mShape.getPosition().y - 10);
    mHealthBar.setSize(sf::Vector2f(40 * (mHealth / 60.0f), 5));

    if (mBurnDuration > 0)
    {
        mTimeSinceLastBurn += dt;
        if (mTimeSinceLastBurn >= 1.0f)  // Apply burn damage every second
        {
            takeDamage(mBurnDamage);
            mTimeSinceLastBurn = 0.0f;
            mBurnDuration -= 1.0f;  // Decrease burn duration
        }
    }
}

bool Enemy::hasReachedEnd() const
{
    return mCurrentWaypoint >= mPath.size();
}

sf::Vector2f Enemy::getPosition() const
{
    if (!mShape.getGlobalBounds().width || !mShape.getGlobalBounds().height) {
        std::cerr << "mShape is not properly initialized! " << mShape.getGlobalBounds().width << " " <<
            mShape.getGlobalBounds().height << std::endl;
        return sf::Vector2f(0, 0);
    }
    return mShape.getPosition();
}

void Enemy::takeDamage(float amount)
{
    mHealth -= amount;
    if (mHealth <= 0) {
        mHealth = 0;
    }
}

bool Enemy::isDead() const
{
    return mHealth <= 0;
}

float Enemy::getSpawnTime() const
{
    return mSpawnTime;
}

int Enemy::getReward() const
{
    return mReward;
}

float Enemy::getDamageToPlayer() const
{
    return mDamageToPlayer;
}

float Enemy::getDamageToTower() const
{
    return mDamageToGame;
}

float Enemy::getAttackRange() const
{
    return mAttackRange;
}

float Enemy::getFollowRange() const
{
    return mFollowRange;
}

float Enemy::getAttackSpeed() const
{
    return mAttackSpeed;
}

sf::Vector2f Enemy::getSize() const
{
    return sf::Vector2f(mShape.getRadius(), mShape.getRadius());
}

bool Enemy::containsPoint(const sf::Vector2f& point) const
{
    return mShape.getGlobalBounds().contains(point);
}

sf::FloatRect Enemy::getBounds() const
{
    return mShape.getGlobalBounds();
}

void Enemy::render(sf::RenderWindow& window)
{
    window.draw(mShape);
    window.draw(mHealthBar);
}

void Enemy::followPath(float dt)
{
    if (mCurrentWaypoint < mPath.size()) {
        sf::Vector2f currentPos = mShape.getPosition();
        sf::Vector2f targetPos = mPath[mCurrentWaypoint];
        sf::Vector2f direction = targetPos - currentPos;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < mSpeed * dt)
        {
            mShape.setPosition(targetPos);
            mCurrentWaypoint++;
        }
        else
        {
            direction /= distance;
            mShape.move(direction * mSpeed * dt);
        }
    }
}

void Enemy::followPlayer(float dt, Player& player)
{
    sf::Vector2f currentPos = mShape.getPosition();
    sf::Vector2f playerPos = player.getPosition();
    sf::Vector2f direction = playerPos - currentPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    direction /= distance;
    mShape.move(direction * mSpeed * dt);
}

void Enemy::attackPlayer(float dt, Player& player)
{
    mTimeSinceLastAttack += dt;

    if (mTimeSinceLastAttack >= mAttackSpeed)
    {
        player.takeDamage(mDamageToPlayer);
        mTimeSinceLastAttack = 0.0f;
    }
}

void Enemy::applyBurn(float damage, float duration)
{
    mBurnDamage = damage;
    mBurnDuration = duration;
    mTimeSinceLastBurn = 0.0f;
}
