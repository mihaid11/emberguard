#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include "../projectiles/Projectile.h"
#include "../enemy/Enemy.h"
#include "Upgrade.h"

class Tower {
public:
    Tower(const sf::Vector2f& position, std::vector <Projectile>& projectiles);
    virtual ~Tower() = default;

    virtual void update(float dt, std::vector<Enemy>& enemies);
    void render(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& position);
    Enemy* getTarget() const;

    bool containsPoint(const sf::Vector2f& point) const;
    void setSelected(bool selected);

    int getCost() const;
    sf::Vector2f getSize() const;
    float getDamage() const;
    float getFireRate() const;
    int getSellPrice() const;

    //Upgrade methods
    void upgrade(size_t index, int& crystals);
    bool canUpgrade(size_t index) const;
    float getRange() const;
    size_t getUpgradeCount() const;

    // Add methods to manage upgrades
    void addUpgrade(const Upgrade& upgrade);
    const Upgrade& getUpgrade(size_t index) const;
    int sell();

protected:
    sf::CircleShape mShape;
    float mRange;
    float mFireRate;
    float mTimeSinceLastShot;
    Enemy* mTarget;
    bool mSelected;
    int mCost;
    float mDamage;
    std::vector<Projectile>& mProjectiles;

    void fireProjectile();

    std::vector<Upgrade> mUpgrades;
};

