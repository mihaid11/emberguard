#include "Player.h"
#include <iostream>
#include <cmath>

static float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

Player::Player(const sf::Vector2f& position, std::vector<Projectile>& projectiles)
    : mPosition(position), mTarget(nullptr), mFireRate(1.25f), mTimeSinceLastShot(0.0f),
    mRange(40.0f), mProjectiles(projectiles), mIsMoving(false) {

    mCollisionZone.setSize(sf::Vector2f(26.8f, 50.f));
    mCollisionZone.setPosition(sf::Vector2f(mPosition.x + 13.6f, mPosition.y + 1.f));
    mCollisionZone.setFillColor(sf::Color::Red);

    mSprite.setTextureRect({ 0, 0, 64, 64 });
    mSprite.setScale(0.81f, 0.81f);
    mAnimations[int(AnimationIndex::IdleUp)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/idleUp.png", 4, 2.5f);
    mAnimations[int(AnimationIndex::IdleDown)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/idleDown.png", 6, 1.8f);
    mAnimations[int(AnimationIndex::IdleLeft)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/idleLeft.png", 6, 2.f);
    mAnimations[int(AnimationIndex::IdleRight)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/idleRight.png", 6, 2.f);
    mAnimations[int(AnimationIndex::WalkingUp)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/walkUp.png", 6, 1.6f);
    mAnimations[int(AnimationIndex::WalkingDown)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/walkDown.png", 6, 1.6f);
    mAnimations[int(AnimationIndex::WalkingLeft)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/walkLeft.png", 6, 1.52f);
    mAnimations[int(AnimationIndex::WalkingRight)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/walkRight.png", 6, 1.52f);

    mSpeed = 100.0f;
    mDamage = 10.0f;
    mHealth = 100.0f;
}

void Player::update(float dt, const std::vector<Enemy>& enemies, const sf::RectangleShape& topBorder, const sf::RectangleShape& bottomBorder,
                    const sf::RectangleShape& leftBorder, const sf::RectangleShape& rightBorder) {

    mIsMoving = false;
    bool canMoveRight = true;
    bool canMoveDown = true;
    bool canMoveLeft = true;
    bool canMoveUp = true;

    sf::Vector2f direction(0.f, 0.f);

    // Make a stack of all of the key inputs to know which one was last
    static std::vector<sf::Keyboard::Key> keyOrder;

    auto addKey = [&](sf::Keyboard::Key key) {
        if (std::find(keyOrder.begin(), keyOrder.end(), key) == keyOrder.end())
            keyOrder.push_back(key);
    };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { direction.x -= 1.165f; mIsMoving = true; addKey(sf::Keyboard::A); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { direction.x += 1.165f; mIsMoving = true; addKey(sf::Keyboard::D); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { direction.y -= 1.165f; mIsMoving = true; addKey(sf::Keyboard::W); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { direction.y += 1.165f; mIsMoving = true; addKey(sf::Keyboard::S); }

    auto removeKey = [&](sf::Keyboard::Key key) {
        keyOrder.erase(std::remove(keyOrder.begin(), keyOrder.end(), key), keyOrder.end());
    };

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { removeKey(sf::Keyboard::A); }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { removeKey(sf::Keyboard::D); }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { removeKey(sf::Keyboard::W); }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { removeKey(sf::Keyboard::S); }

    // Get the last key input
    if (!keyOrder.empty())
        mLastDirection = keyOrder.back();

    if (direction.x != 0.f || direction.y != 0.f) {
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length;
    }

    if (direction.x == 0.f && direction.y == 0.f) {
        switch (mLastDirection) {
            case sf::Keyboard::A: mCurrentAnimation = AnimationIndex::IdleLeft; break;
            case sf::Keyboard::D: mCurrentAnimation = AnimationIndex::IdleRight; break;
            case sf::Keyboard::W: mCurrentAnimation = AnimationIndex::IdleUp; break;
            case sf::Keyboard::S: mCurrentAnimation = AnimationIndex::IdleDown; break;
            default: break;
        }
    } else {
        switch (mLastDirection) {
            case sf::Keyboard::A: mCurrentAnimation = AnimationIndex::WalkingLeft; break;
            case sf::Keyboard::D: mCurrentAnimation = AnimationIndex::WalkingRight; break;
            case sf::Keyboard::W: mCurrentAnimation = AnimationIndex::WalkingUp; break;
            case sf::Keyboard::S: mCurrentAnimation = AnimationIndex::WalkingDown; break;
            default: break;
        }
    }

    // Manage border collision
    if (topBorder.getGlobalBounds().intersects(sf::FloatRect(mCollisionZone.getPosition().x,
        mCollisionZone.getPosition().y, mCollisionZone.getSize().x, mCollisionZone.getSize().y))) {
        canMoveUp = false;
    }
    if (bottomBorder.getGlobalBounds().intersects(sf::FloatRect(mCollisionZone.getPosition().x,
        mCollisionZone.getPosition().y, mCollisionZone.getSize().x, mCollisionZone.getSize().y))) {
        canMoveDown = false;
    }
    if (leftBorder.getGlobalBounds().intersects(sf::FloatRect(mCollisionZone.getPosition().x,
        mCollisionZone.getPosition().y, mCollisionZone.getSize().x, mCollisionZone.getSize().y))) {
        canMoveLeft = false;
    }
    if (rightBorder.getGlobalBounds().intersects(sf::FloatRect(mCollisionZone.getPosition().x,
        mCollisionZone.getPosition().y, mCollisionZone.getSize().x, mCollisionZone.getSize().y))) {
        canMoveRight = false;
    }

    mAnimations[int(mCurrentAnimation)].update(dt);
    mAnimations[int(mCurrentAnimation)].applyToSprite(mSprite);

    sf::Vector2f originalPosition = mPosition;
    mPosition += direction * mSpeed * dt;

    if (!canMoveDown)
        mPosition.y = originalPosition.y - 0.01f;
    if (!canMoveRight)
        mPosition.x = originalPosition.x - 0.01f;
    if (!canMoveLeft)
        mPosition.x = originalPosition.x + 0.01f;
    if (!canMoveUp)
        mPosition.y = originalPosition.y + 0.01f;

    mSprite.setPosition(mPosition);
    mCollisionZone.setPosition(sf::Vector2f(mPosition.x + 13.6f, mPosition.y + 1.f));

    // If the player is not moving, check for enemies and fire projectiles
    if (!mIsMoving) {
        mTimeSinceLastShot += dt;

        if (mTimeSinceLastShot >= mFireRate) {
            Enemy* selectedEnemy = nullptr;
            float closestDistance = mRange;

            for (const auto& enemy : enemies) {
                float dist = distance(mSprite.getPosition(), enemy.getPosition());
                if (dist <= mRange && (selectedEnemy == nullptr || enemy.getSpawnTime() < selectedEnemy->getSpawnTime()))
                    selectedEnemy = const_cast<Enemy*>(&enemy);
            }

            mTarget = selectedEnemy;

            if (mTarget) {
                fireProjectile();
                mTimeSinceLastShot = 0.0f;
            }
        }

        if (!mTarget || mTarget->isDead() || distance(mSprite.getPosition(), mTarget->getPosition()) > mRange)
            mTarget = nullptr;
    }
}

void Player::render(sf::RenderWindow& window) {
    // Show collision zone for debugging option
    //window.draw(mCollisionZone);
    window.draw(mSprite);
}

sf::Vector2f Player::getPosition() {
    return mCollisionZone.getPosition();
}

bool Player::isInRange(const Enemy& enemy) const {
    float range = mRange;
    float distance = std::sqrt(std::pow(mCollisionZone.getPosition().x +
                     mCollisionZone.getSize().x / 2.f - enemy.getPosition().x, 2.f) +
                     std::pow(mCollisionZone.getPosition().y + mCollisionZone.getSize().y / 2.f -
                     enemy.getPosition().y, 2.f));
    return distance <= range;
}

float Player::getAttackRange() const {
    return mRange;
}

float Player::getPlacementRange() const {
    return PlacementRange;
}

sf::Vector2f Player::getPosition() const {
    return mSprite.getPosition();
}

float Player::getDamage() const {
    return mDamage;
}

float Player::getFireRate() const {
    return mFireRate;
}

sf::FloatRect Player::getBounds() const {
    return mSprite.getGlobalBounds();
}

void Player::setPosition(const sf::Vector2f& position) {
    mSprite.setPosition(position);
    mPosition = position;
}

void Player::fireProjectile() {
    if (!mTarget) return;

    sf::Vector2f targetPos = mTarget->getPosition();
    sf::Vector2f direction = targetPos - sf::Vector2f(mCollisionZone.getPosition().x,
        mCollisionZone.getPosition().y + mCollisionZone.getSize().y / 2.6f);
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= magnitude;

    sf::Vector2f projectilePos = sf::Vector2f(mCollisionZone.getPosition().x,
        mCollisionZone.getPosition().y + mCollisionZone.getSize().y / 2.6f);
    mProjectiles.emplace_back(projectilePos, mTarget, 200.0f, 5.0f);
}

void Player::takeDamage(float amount) {
    mHealth -= amount;
    if (mHealth < 0)
        mHealth = 0;
}

float Player::getHealth() const {
    return mHealth;
}

void Player::setHealth(float health) {
    mHealth = health;
}

