#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class PlayerMenu
{
public:
    PlayerMenu();

    void render(sf::RenderWindow& window, const Player& player);
    bool isMouseOverIcon(const sf::Vector2f& mousePos) const;
    void updateHealthBar(float health);
    void setPosition(const sf::Vector2f& position);
    void selectPlayer(bool select);
    void toggleSelect();
    bool isVisible() const;

private:
    sf::RectangleShape mIconBackground;
    sf::RectangleShape mHealthBar;
    sf::RectangleShape mHealthBarBackground;
    sf::CircleShape mPlayerIcon;
    sf::CircleShape mAttackRangeCircle;
    sf::CircleShape mPlacementRangeCircle;
    bool mIsSelected;
    sf::Vector2f mPosition;

    sf::RectangleShape mStatsRectangle;
    sf::Text mDamageText;
    sf::Text mFireRateText;
    sf::Text mRangeText;
    sf::Text mPlacementRangeText;
    sf::Vector2f mStatsPosition;
    sf::Font mFont;
    void formatStats(const Player& player);
};

