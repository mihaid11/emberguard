#pragma once
#include <SFML/Graphics.hpp>
#include "Tower.h"
#include "LaserTower.h"
#include "Upgrade.h"

class TowerMenu {
public:
    TowerMenu();

    void show(const Tower& tower);
    void hide();
    bool isVisible() const;
    void update(const sf::Vector2f& mousePos, int& crystals, const sf::Vector2f& position, const sf::FloatRect& size);
    void render(sf::RenderWindow& window);
    bool containsUpgradeZone(const sf::Vector2f& point) const;
    void handleUpgradeClick(const sf::Vector2f& mousePos, Tower& tower, int& crystals);
    int findUpgradeZone(const sf::Vector2f& point) const;
    void handleSellClick(Tower& tower, int& crystals);

private:
    bool mVisible;
    const Tower* mCurrentTower;

    sf::CircleShape mRangeCircle;
    sf::ConvexShape mHeptagon;
    std::vector<sf::ConvexShape> mZones;
    std::vector<sf::Text> mUpgradeLabels;
    sf::Font mFont;
    int mNumberOfUpgrades;

    sf::RectangleShape mStatsRectangle;
    sf::CircleShape mTowerIcon;
    sf::Text mDamageText;
    sf::Text mFireRateText;
    sf::Text mRangeText;
    sf::Text mSellPriceText;
    sf::Vector2f mStatsPosition;

    bool mIncrementVisible;
    sf::Text mIncrementText;
    std::string mIncrementString;
    sf::Vector2f mIncrementPosition;
    sf::Text mDecrementText;
    std::string mDecrementString;
    sf::Vector2f mDecrementPosition;

    void initializeHeptagon(const sf::Vector2f& position, float radius);
    void initializeZones();
    void initializeLabels();
    void initializeStats();
    void formatStats();
    bool pointInZone(const sf::Vector2f& point, const sf::ConvexShape& zone) const;
    void drawLines(sf::RenderWindow& window);
    void updateHoverEffect(const sf::Vector2f& mousePos, int& crystals, const sf::Vector2f& position, const sf::FloatRect& size);
    void showStats(const Tower& tower);
};

