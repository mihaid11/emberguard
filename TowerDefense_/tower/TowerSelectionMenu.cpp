#include "TowerSelectionMenu.h"
#include <iostream>
#include <cmath>

const float PI = 3.14159265358979323846f;

TowerSelectionMenu::TowerSelectionMenu() : mVisible(false), mNumberOfTowers(6), mMenuRadius(70.0f) {
    if (!mFont.loadFromFile("../assests/fonts/gameFont.ttf"))
        std::cout << "Couldn't load font2" << std::endl;

    initializeHexagon();
    initializeLabels();
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
        label.setString(std::to_string(i));
        label.setCharacterSize(15);
        label.setFillColor(sf::Color::White);

        float angle = i * 2.0f * PI / mNumberOfTowers + PI / mNumberOfTowers;
        sf::Vector2f offset(std::cos(angle) * (mMenuRadius * 0.5f), std::sin(angle) * (mMenuRadius * 0.5f));
        label.setPosition(mPosition + offset - sf::Vector2f(10, 10));

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


void TowerSelectionMenu::show(const sf::Vector2f& position)
{
    mPosition = position;
    mHexagon.setPosition(mPosition);
    initializeLabels();
    initializeZones();
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
    drawZones(window);
    window.draw(mHexagon);
    drawLines(window);
    for (const auto& label : mTowerLabels) {
        window.draw(label);
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

void TowerSelectionMenu::updateHover(const sf::Vector2f& mousePosition)
{
    mHoveredZone = -1;

    for (int i = 0; i < mNumberOfTowers; ++i)
    {
        const sf::ConvexShape& zone = mZoneShapes[i];

        if (zone.getPointCount() > 0 && isPointInConvexShape(zone, mousePosition))
        {
            mHoveredZone = i;
            break;
        }
    }
}

void TowerSelectionMenu::drawZones(sf::RenderWindow& window) const
{
    sf::Color normalColor(0, 0, 0, 200);
    sf::Color hoverColor(192, 192, 192, 255);

    for (int i = 0; i < mNumberOfTowers; ++i)
    {
        const sf::ConvexShape& zone = mZoneShapes[i];

        sf::ConvexShape drawableZone = zone;
        drawableZone.setFillColor((i == mHoveredZone) ? hoverColor : normalColor);

        window.draw(drawableZone);
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

