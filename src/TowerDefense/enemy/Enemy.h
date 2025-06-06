#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Player;

class Enemy
{
public:
    Enemy(const std::vector<sf::Vector2f>& path);

    void update(float dt, Player& player);
    void render(sf::RenderWindow& window);
    bool hasReachedEnd() const;
    sf::Vector2f getPosition() const;
    void takeDamage(float amount);
    bool isDead() const;
    float getSpawnTime() const;
    int getReward() const;

    float getDamageToPlayer() const;
    float getDamageToTower() const;
    float getAttackRange() const;
    float getFollowRange() const;
    float getAttackSpeed() const;
    sf::Vector2f getSize() const;

    sf::FloatRect getBounds() const;
    bool containsPoint(const sf::Vector2f& point) const;

    void applyBurn(float damage, float duration);

protected:

    std::vector<sf::Vector2f> mPath;
    std::size_t mCurrentWaypoint;
    sf::CircleShape mShape;
    sf::RectangleShape mHealthBar;

    //Atributes
    float mSpeed;
    float mHealth;
    float mSpawnTime;
    float mDamageToPlayer;
    float mDamageToGame;
    float mAttackSpeed;
    float mAttackRange;
    float mFollowRange;
    float mTimeSinceLastAttack;
    int mReward;

    float mBurnDamage;
    float mBurnDuration;
    float mTimeSinceLastBurn;

    void followPath(float dt);
    void followPlayer(float dt, Player& player);
    void attackPlayer(float dt, Player& player);
};
