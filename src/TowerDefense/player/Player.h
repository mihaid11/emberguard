#pragma once
#include <SFML/Graphics.hpp>
#include "../enemy/Enemy.h"
#include <vector>
#include "../projectiles/Projectile.h"
#include "../../Animation.h"

class Player
{
public:
    Player(const sf::Vector2f& position, std::vector<Projectile>& projectiles);

    void update(float dt, const std::vector<Enemy>& enemies, const sf::RectangleShape& topBorder, const sf::RectangleShape& bottomBorder,
        const sf::RectangleShape& leftBorder, const sf::RectangleShape& rightBorder);
    void render(sf::RenderWindow& window);
    sf::Vector2f getPosition();
    bool isInRange(const Enemy& enemy) const;

    float getAttackRange() const;
    float getPlacementRange() const;
    sf::Vector2f getPosition() const;
    float getDamage() const;
    float getFireRate() const;

    void takeDamage(float amount);
    float getHealth() const;
    void setHealth(float health);

    sf::FloatRect getBounds() const;
    void setPosition(const sf::Vector2f& position);

private:
    sf::Vector2f mPosition;
    Enemy* mTarget;
    float mSpeed;
    float mDamage;
    float mFireRate;
    float mTimeSinceLastShot;
    float mRange;
    std::vector<Projectile>& mProjectiles;
    bool mIsMoving;
    float mHealth;
    static constexpr float PlacementRange = 180.0f;

    enum class AnimationIndex {
        IdleUp,
        IdleDown,
        IdleLeft,
        IdleRight,
        WalkingUp,
        WalkingDown,
        WalkingLeft,
        WalkingRight,
        Count
    };

    sf::Sprite mSprite;
    Animation mAnimations[int(AnimationIndex::Count)];
    AnimationIndex mCurrentAnimation = AnimationIndex::IdleDown;
    sf::RectangleShape mCollisionZone;
    sf::Keyboard::Key mLastDirection = sf::Keyboard::A;

    void fireProjectile();
};

