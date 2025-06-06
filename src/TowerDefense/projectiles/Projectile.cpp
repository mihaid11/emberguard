#include "Projectile.h"
#include <cmath>
#include <iostream>

Projectile::Projectile(const sf::Vector2f& position, Enemy* target, float speed, float damage)
    : mTarget(target), mSpeed(speed), mDamage(damage), mHitTarget(false), mAlive(true)
{
    if (target == nullptr) {
        std::cerr << "Error: target is null in Projectile constructor" << std::endl;
        return;
    }

    mShape.setSize(sf::Vector2f(10, 5));
    mShape.setFillColor(sf::Color::Red);
    mShape.setPosition(position);

    // Set origin to the center of the shape for correct rotation
    mShape.setOrigin(mShape.getSize() / 2.0f);

    // Calculate direction to target
    sf::Vector2f direction = target->getPosition() - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= length;

    // Set the rotation of the bullet
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159f;
    mShape.setRotation(angle);
}

void Projectile::update(float dt)
{
    if (mTarget == nullptr) {
        std::cerr << "Warning: target is null in Projectile update" << std::endl;
        return;
    }

    if (mTarget->isDead()) {
        mHitTarget = true; // Mark as hit so it can be removed
        return; // Stop updating this projectile
    }

    // Move the projectile towards the target
    sf::Vector2f direction = mTarget->getPosition() - mShape.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0f) {
        direction /= length;
        sf::Vector2f movement = direction * mSpeed * dt;
        mShape.move(movement);

        // Check if the projectile has hit the target
        if (!mHitTarget && checkCollision(mTarget)) {
            //mTarget->takeDamage(mDamage);
            mHitTarget = true;
        }
    }
}

bool Projectile::checkCollision(const Enemy* target) const
{
    sf::FloatRect projectileBounds = mShape.getGlobalBounds();
    sf::FloatRect targetBounds = target->getBounds();

    return projectileBounds.intersects(targetBounds);
}


void Projectile::render(sf::RenderWindow& window)
{
    if (mTarget == nullptr) {
        std::cerr << "Warning: target is null in Projectile render" << std::endl;
        return;
    }
    window.draw(mShape);
}

bool Projectile::hasHitTarget() const
{
    return mHitTarget;
}

sf::Vector2f Projectile::getPosition() const
{
    return mShape.getPosition();
}

float Projectile::getDamage() const
{
    return mDamage;
}

Enemy* Projectile::getTarget() const
{
    return mTarget;
}
