#include "FlameProjectile.h"
#include <cmath>

FlameProjectile::FlameProjectile(const sf::Vector2f& position, Enemy* target, float speed, float damage)
    : Projectile(position, target, speed, damage), mBurnDuration(2.0f), mGroundBurned(false) {

    mShape.setPointCount(3);
    mShape.setPoint(0, sf::Vector2f(0, 0));
    mShape.setPoint(1, sf::Vector2f(-10.0f, 20.0f)); 
    mShape.setPoint(2, sf::Vector2f(10.0f, 20.0f));
    mShape.setFillColor(sf::Color(255, 69, 0, 128)); 
    mShape.setPosition(position);
}

void FlameProjectile::update(float dt) {
    if (mTarget) {
        sf::Vector2f direction = mTarget->getPosition() - mShape.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < mSpeed * dt) {
            mTarget->takeDamage(mDamage);
            mTarget->applyBurn(mDamage * 0.1f, mBurnDuration);  
            mAlive = false;
        }
        else {
            direction /= distance;  // Normalize
            mShape.move(direction * mSpeed * dt);
        }

        // Set the ground on fire
        if (!mGroundBurned) {
            //TODO : Add logic to the burning ground
            mGroundBurned = true;
        }
    }
}

void FlameProjectile::render(sf::RenderWindow& window)
{
    window.draw(mShape);
}

