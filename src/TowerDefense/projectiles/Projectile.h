#pragma once
#include <SFML/Graphics.hpp>
#include "../enemy/Enemy.h"

class Projectile
{
public:
    Projectile(const sf::Vector2f& position, Enemy* target, float speed, float damage);

    virtual void update(float dt);
    virtual void render(sf::RenderWindow& window);

    bool hasHitTarget() const;
    sf::Vector2f getPosition() const;
    float getDamage() const;
    Enemy* getTarget() const;

protected:
    sf::RectangleShape mShape;
    Enemy* mTarget;
    float mSpeed;
    float mDamage;
    bool mHitTarget;
    bool mAlive;

    bool checkCollision(const Enemy* target) const;
};

