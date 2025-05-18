#include "PlayerMenu.h"
#include <iostream>
#include <sstream>

PlayerMenu::PlayerMenu() : mIsSelected(false)
{
    mIconBackground.setSize(sf::Vector2f(130, 25));
    mIconBackground.setFillColor(sf::Color(50, 50, 50));

    mHealthBarBackground.setSize(sf::Vector2f(5, 25));
    mHealthBarBackground.setFillColor(sf::Color::Red);

    mPlayerIcon.setRadius(10);
    mPlayerIcon.setFillColor(sf::Color::Cyan);

    mHealthBar.setSize(sf::Vector2f(5, 25));
    mHealthBar.setFillColor(sf::Color::Green);

    mAttackRangeCircle.setFillColor(sf::Color(255, 0, 0, 50));
    mPlacementRangeCircle.setFillColor(sf::Color(0, 0, 255, 50));

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }

    mStatsRectangle.setSize(sf::Vector2f(400, 70));
    mStatsRectangle.setFillColor(sf::Color(50, 50, 50, 200));
    mStatsRectangle.setOutlineThickness(2);
    mStatsRectangle.setOutlineColor(sf::Color::White);

    mDamageText.setFont(mFont);
    mDamageText.setCharacterSize(12);
    mDamageText.setFillColor(sf::Color::White);

    mFireRateText.setFont(mFont);
    mFireRateText.setCharacterSize(12);
    mFireRateText.setFillColor(sf::Color::White);

    mRangeText.setFont(mFont);
    mRangeText.setCharacterSize(12);
    mRangeText.setFillColor(sf::Color::White);

    mPlacementRangeText.setFont(mFont);
    mPlacementRangeText.setCharacterSize(12);
    mPlacementRangeText.setFillColor(sf::Color::White);

    mStatsPosition = sf::Vector2f(700, 10);
}

void PlayerMenu::render(sf::RenderWindow& window, const Player& player)
{
    window.draw(mIconBackground);
    window.draw(mPlayerIcon);
    window.draw(mHealthBarBackground);
    window.draw(mHealthBar);

    if (!mIsSelected) return;

    mAttackRangeCircle.setRadius(player.getAttackRange());
    mPlacementRangeCircle.setRadius(player.getPlacementRange());

    mAttackRangeCircle.setOrigin(mAttackRangeCircle.getRadius(), mAttackRangeCircle.getRadius());
    mPlacementRangeCircle.setOrigin(mPlacementRangeCircle.getRadius(), mPlacementRangeCircle.getRadius());

    // Hardcoded values for text and circle position, to be changed later
    mAttackRangeCircle.setPosition(player.getPosition().x + 26.5f, player.getPosition().y + 25.4f);
    mPlacementRangeCircle.setPosition(player.getPosition().x + 26.5f, player.getPosition().y + 25.4f);

    mStatsRectangle.setPosition(mStatsPosition);
    mDamageText.setPosition(mStatsRectangle.getPosition() + sf::Vector2f(115, 20));
    mFireRateText.setPosition(mStatsRectangle.getPosition() + sf::Vector2f(230, 20));
    mRangeText.setPosition(mStatsRectangle.getPosition() + sf::Vector2f(115, 35));
    mPlacementRangeText.setPosition(mStatsRectangle.getPosition() + sf::Vector2f(230, 35));

    formatStats(player);

    window.draw(mPlacementRangeCircle);
    window.draw(mAttackRangeCircle);

    window.draw(mStatsRectangle);
    window.draw(mDamageText);
    window.draw(mFireRateText);
    window.draw(mRangeText);
    window.draw(mPlacementRangeText);
}

bool PlayerMenu::isMouseOverIcon(const sf::Vector2f& mousePos) const
{
    return mIconBackground.getGlobalBounds().contains(mousePos);
}

void PlayerMenu::updateHealthBar(float health)
{
    float healthPercentage = std::max(health / 100.0f, 0.0f);
    float fullHeight = 25.0f;
    float currentHeight = fullHeight * healthPercentage;

    // Update health bar size and position //
    mHealthBar.setSize(sf::Vector2f(5, currentHeight));
}

void PlayerMenu::setPosition(const sf::Vector2f& position)
{
    mPosition = position;
    mIconBackground.setPosition(position);

    mPlayerIcon.setPosition(position.x + 5, position.y + 2.5f);
    mHealthBar.setPosition(position.x + 126, position.y);
    mHealthBarBackground.setPosition(mHealthBar.getPosition());
}

void PlayerMenu::selectPlayer(bool select)
{
    mIsSelected = select;
    mIconBackground.setOutlineThickness(select ? 1.0f : 0.0f);
    mIconBackground.setOutlineColor(sf::Color::Yellow);
}

void PlayerMenu::toggleSelect()
{
    mIsSelected = !mIsSelected;
    mIconBackground.setOutlineThickness(mIsSelected ? 1.0f : 0.0f);
    mIconBackground.setOutlineColor(sf::Color::Yellow);
}

bool PlayerMenu::isVisible() const
{
    return mIsSelected;
}

void PlayerMenu::formatStats(const Player& player)
{
    std::ostringstream damageStream;
    damageStream.precision(2);
    damageStream << std::fixed << player.getDamage();
    mDamageText.setString("Damage " + damageStream.str());

    std::ostringstream fireRateStream;
    fireRateStream.precision(2);
    fireRateStream << std::fixed << player.getFireRate();
    mFireRateText.setString("Fire Rate " + fireRateStream.str());

    std::ostringstream rangeStream;
    rangeStream.precision(2);
    float range = player.getAttackRange();
    rangeStream << std::fixed << range;
    mRangeText.setString("Range " + rangeStream.str());

    std::ostringstream placementStream;
    placementStream.precision(2);
    placementStream << std::fixed << player.getPlacementRange();
    mPlacementRangeText.setString("Place Range " + placementStream.str());
}
