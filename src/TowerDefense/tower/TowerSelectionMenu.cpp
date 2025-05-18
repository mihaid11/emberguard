#include "TowerSelectionMenu.h"
#include <iostream>
#include <cmath>
#include <sstream>

const float PI = 3.14159265358979323846f;

TowerSelectionMenu::TowerSelectionMenu(std::vector<int>& availableTowers) 
    : mVisible(false), mNumberOfTowers(6), mMenuRadius(70.0f), mAvailableTowers(availableTowers) {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Couldn't load font2 ffsk" << std::endl;
    initializeHexagon();
    initializeLabels();
    initializeStats();
    mTowerIcon.setRadius(15);
    mTowerIcon.setFillColor(sf::Color::Green);

}

void TowerSelectionMenu::initializeHexagon() {
    mHexagon.setPointCount(6);
    for (int i = 0; i < 6; ++i) {
        float angle = i * 2.0f * PI / 6.0f;
        sf::Vector2f point(mMenuRadius * std::cos(angle), mMenuRadius * std::sin(angle));
        mHexagon.setPoint(i, point);
    }
    mHexagon.setFillColor(sf::Color(0, 0, 0, 200));
    mHexagon.setOutlineThickness(2);
    mHexagon.setOutlineColor(sf::Color::White);
}

void TowerSelectionMenu::initializeLabels() {
    mTowerLabels.clear();
    for (int i = 0; i < mNumberOfTowers; ++i) {

        sf::Text label;
        label.setFont(mFont);
        if (i < mAvailableTowers.size())
            if (mAvailableTowers[i] == 1)
                label.setString("Laser");
            else if (mAvailableTowers[i] == 2)
                label.setString("Flame");
            else
                label.setString("");
        else
            label.setString("");
        label.setCharacterSize(11);
        label.setFillColor(sf::Color::White);

        float angle = i * 2.0f * PI / mNumberOfTowers + PI / mNumberOfTowers;
        sf::Vector2f offset(std::cos(angle) * (mMenuRadius * 0.5f), std::sin(angle) * (mMenuRadius * 0.5f));
        label.setPosition(mPosition + offset - sf::Vector2f(10, 10));
        if (i == 0)
            label.setPosition(sf::Vector2f(label.getPosition().x - 1.5f, label.getPosition().y + 3.5f));
        if (i == 1)
            label.setPosition(sf::Vector2f(label.getPosition().x - 5.f, label.getPosition().y + 5.5f));
        else if (i == 2)
            label.setPosition(sf::Vector2f(label.getPosition().x - 10.f, label.getPosition().y + 4.5f));
        else if (i == 3)
            label.setPosition(sf::Vector2f(label.getPosition().x - 10.f, label.getPosition().y - 2.f));
        else if (i == 4)
            label.setPosition(sf::Vector2f(label.getPosition().x - 7.5f, label.getPosition().y - 1.5f));
        mTowerLabels.push_back(label);
    }
}

void TowerSelectionMenu::initializeZones() {
    mZoneShapes.clear();
    float sectorAngle = 2 * PI / mNumberOfTowers;

    for (int i = 0; i < mNumberOfTowers; ++i) {
        sf::ConvexShape zone;
        zone.setPointCount(3);

        zone.setPoint(0, mPosition);

        sf::Vector2f corner1 = mPosition + mHexagon.getPoint(i);
        sf::Vector2f corner2 = mPosition + mHexagon.getPoint((i + 1) % 6);

        zone.setPoint(1, corner1);
        zone.setPoint(2, corner2);

        zone.setFillColor(sf::Color(0, 0, 0, 200));

        mZoneShapes.push_back(zone);
    }
}

void TowerSelectionMenu::initializeStats()
{
    mStatsRectangle.setSize(sf::Vector2f(430, 70));
    mStatsRectangle.setFillColor(sf::Color(50, 50, 50, 200));
    mStatsRectangle.setOutlineThickness(2);
    mStatsRectangle.setOutlineColor(sf::Color::White);

    mDamageText.setFont(mFont);
    mDamageText.setCharacterSize(12);
    mDamageText.setFillColor(sf::Color::White);
    mDamageText.setString("Damage: ");

    mFireRateText.setFont(mFont);
    mFireRateText.setCharacterSize(12);
    mFireRateText.setFillColor(sf::Color::White);
    mFireRateText.setString("Fire Rate: ");

    mRangeText.setFont(mFont);
    mRangeText.setCharacterSize(12);
    mRangeText.setFillColor(sf::Color::White);
    mFireRateText.setString("Range: ");

    mSellPriceText.setFont(mFont);
    mSellPriceText.setCharacterSize(12);
    mSellPriceText.setFillColor(sf::Color::White);
    mSellPriceText.setString("Sell price: ");

    mStatsRectangle.setPosition(sf::Vector2f(700.f, 10.f));
    mDamageText.setPosition(mStatsRectangle.getPosition() + sf::Vector2f(115, 20));
    mFireRateText.setPosition(mStatsRectangle.getPosition() + sf::Vector2f(270, 20));
    mRangeText.setPosition(mStatsRectangle.getPosition() + sf::Vector2f(115, 35));
    mSellPriceText.setPosition(mStatsRectangle.getPosition() + sf::Vector2f(270, 35));
    mTowerIcon.setPosition(mStatsRectangle.getPosition() + sf::Vector2f(25, 20));
}


void TowerSelectionMenu::show(const sf::Vector2f& position)
{
    mPosition = position;
    mHexagon.setPosition(mPosition);
    initializeLabels();
    initializeZones();
    initializeStats();
    mVisible = true;
}

void TowerSelectionMenu::hide()
{
    mVisible = false;
}

bool TowerSelectionMenu::isVisible() const
{
    return mVisible;
}

void TowerSelectionMenu::render(sf::RenderWindow& window)
{
    if (!mVisible) return;
    for (auto& zone : mZoneShapes)
        window.draw(zone);
    window.draw(mHexagon);
    drawLines(window);
    for (const auto& label : mTowerLabels) {
        window.draw(label);
    }

    if (mHoveredZone < mAvailableTowers.size()) {
        window.draw(mStatsRectangle);
        window.draw(mDamageText);
        window.draw(mFireRateText);
        window.draw(mRangeText);
        window.draw(mSellPriceText);
        window.draw(mTowerIcon);
    }
}

int TowerSelectionMenu::getSelectedTowerType(const sf::Vector2f& point) const {
    float dx = point.x - mPosition.x;
    float dy = point.y - mPosition.y;
    float angle = std::atan2(dy, dx) * 180.0f / 3.14f;
    angle = angle < 0 ? angle + 360.0f : angle;

    int index = static_cast<int>(angle / (360.0f / mNumberOfTowers));
    return index;
}

bool TowerSelectionMenu::isInsideMenu(const sf::Vector2f& point) const
{
    float dx = point.x - mPosition.x;
    float dy = point.y - mPosition.y;
    return (dx * dx + dy * dy) <= (mMenuRadius * mMenuRadius);
}

void TowerSelectionMenu::updateHover(const sf::Vector2f& mousePosition, int crystals)
{
    mHoveredZone = -1;

    for (int i = 0; i < mNumberOfTowers; ++i)
    {
        sf::ConvexShape& zone = mZoneShapes[i];

        if (i < mAvailableTowers.size())
        {
            int towerCost = 0;
            if (mAvailableTowers[i] == 1) {
                towerCost = 100;
            }
            else if (mAvailableTowers[i] == 2) {
                towerCost = 150;
            }

            sf::Color normalColor;

            if (crystals < towerCost) {
                normalColor = sf::Color(130, 130, 139, 255);
            }
            else {
                normalColor = sf::Color(0, 255, 0, 255);
            }

            // If the mouse is hovering over this zone, lighten the color
            if (zone.getPointCount() > 0 && isPointInConvexShape(zone, mousePosition))
            {
                formatStats(i);
                mHoveredZone = i;
                sf::Color hoverColor = normalColor;
                hoverColor.r = std::min(hoverColor.r + 50, 255);
                hoverColor.g = std::min(hoverColor.g + 50, 255);
                hoverColor.b = std::min(hoverColor.b + 50, 255);

                zone.setFillColor(hoverColor);
            }
            else {
                zone.setFillColor(normalColor);
            }
        }
        else {
            sf::Color normalColor = sf::Color(0, 0, 0, 255);
            zone.setFillColor(normalColor);
        }
    }
}

void TowerSelectionMenu::drawLines(sf::RenderWindow& window) const
{
    sf::VertexArray lines(sf::Lines, 12);

    for (int i = 0; i < 6; ++i)
    {
        lines[i * 2].position = mPosition;
        lines[i * 2].color = sf::Color::White;

        sf::Vector2f corner = mPosition + mHexagon.getPoint(i);
        lines[i * 2 + 1].position = corner;
        lines[i * 2 + 1].color = sf::Color::White;
    }

    window.draw(lines);
}

void TowerSelectionMenu::formatStats(int i)
{
    std::ostringstream damageStream;
    damageStream.precision(2);
    switch (mAvailableTowers[i]) {
    case 1: damageStream << std::fixed << 5.00; break;
    case 2: damageStream << std::fixed << 10.00; break;
    default: damageStream << std::fixed << 0; break;
    }
    mDamageText.setString("Damage " + damageStream.str());

    std::ostringstream fireRateStream;
    fireRateStream.precision(2);
    switch (mAvailableTowers[i]) {
    case 1: fireRateStream << std::fixed << 1.25; break;
    case 2: fireRateStream << std::fixed << 2.00; break;
    default: fireRateStream << std::fixed << 1.0; break;
    }
    mFireRateText.setString("Fire Rate " + fireRateStream.str());

    std::ostringstream rangeStream;
    rangeStream.precision(2);
    switch (mAvailableTowers[i]) {
    case 1: rangeStream << std::fixed << 120.00; break;
    case 2: rangeStream << std::fixed << 100.00; break;
    default: rangeStream << std::fixed << 15; break;
    }
    mRangeText.setString("Range " + rangeStream.str());

    std::ostringstream sellPriceStream;
    sellPriceStream.precision(2);
    switch (mAvailableTowers[i]) {
    case 1: sellPriceStream << std::fixed << 40; break;
    case 2: sellPriceStream << std::fixed << 60; break;
    default: sellPriceStream << std::fixed << 0; break;
    }
    mSellPriceText.setString("Sell Price " + sellPriceStream.str());

    if (mAvailableTowers[i] == 2)
        mTowerIcon.setFillColor(sf::Color(255, 84, 84, 255));
    else if(mAvailableTowers[i] == 1)
        mTowerIcon.setFillColor(sf::Color::Green);
}

bool TowerSelectionMenu::isPointInConvexShape(const sf::ConvexShape& shape, const sf::Vector2f& point) const
{
    std::size_t pointCount = shape.getPointCount();

    if (pointCount < 3)
        return false;

    for (std::size_t i = 0; i < pointCount; ++i)
    {
        sf::Vector2f p1 = shape.getPoint(i);
        sf::Vector2f p2 = shape.getPoint((i + 1) % pointCount);

        float crossProduct = (p2.x - p1.x) * (point.y - p1.y) - (p2.y - p1.y) * (point.x - p1.x);

        if (crossProduct < 0)
            return false;
    }
    return true;
}

