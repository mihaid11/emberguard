#include "TowerMenu.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <math.h>

const float PI = 3.14159265358979323846f;

TowerMenu::TowerMenu() : mVisible(false), mCurrentTower(nullptr), mNumberOfUpgrades(7), mIncrementVisible(false) {
    mRangeCircle.setFillColor(sf::Color::Transparent);
    mRangeCircle.setOutlineColor(sf::Color::Green);
    mRangeCircle.setOutlineThickness(2);

    mTowerIcon.setRadius(15);
    mTowerIcon.setFillColor(sf::Color::Green);

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }

    mStatsPosition = sf::Vector2f(700, 10);
}

void TowerMenu::show(const Tower& tower) {
    mCurrentTower = &tower;
    mVisible = true;

    mRangeCircle.setRadius(tower.getRange());
    mRangeCircle.setOrigin(tower.getRange(), tower.getRange());
    mRangeCircle.setPosition(tower.getPosition().x + tower.getSize().x,
        tower.getPosition().y + tower.getSize().y);

    initializeHeptagon(mRangeCircle.getPosition(), 80.0f);
    initializeZones();
    initializeLabels();
    initializeStats();
    showStats(tower);
}

void TowerMenu::hide() {
    mVisible = false;
    mCurrentTower = nullptr;
}

bool TowerMenu::isVisible() const
{
    return  mVisible;
}

void TowerMenu::initializeHeptagon(const sf::Vector2f& position, float radius) {
    mHeptagon.setPointCount(7);
    for (int i = 0; i < 7; ++i) {
        float angle = i * 2.0f * PI / 7.0f;
        sf::Vector2f point(radius * std::cos(angle), radius * std::sin(angle));
        mHeptagon.setPoint(i, point);
    }
    mHeptagon.setRotation(12.077f);

    mHeptagon.setFillColor(sf::Color(0, 0, 0, 200));
    mHeptagon.setOutlineThickness(2);
    mHeptagon.setOutlineColor(sf::Color::White);
    mHeptagon.setPosition(position);
}

void TowerMenu::initializeZones() {
    mZones.clear();

    for (int i = 0; i < mNumberOfUpgrades; ++i) {
        sf::ConvexShape zone;
        zone.setPointCount(3);

        sf::Vector2f corner1 = mHeptagon.getTransform().transformPoint(mHeptagon.getPoint(i));
        sf::Vector2f corner2 = mHeptagon.getTransform().transformPoint(mHeptagon.getPoint((i + 1) % 7));

        zone.setPoint(0, mHeptagon.getPosition());
        zone.setPoint(1, corner1);
        zone.setPoint(2, corner2);

        if (i != 1) {
            zone.setFillColor(sf::Color(169, 169, 169, 150));
        }
        else {
            zone.setFillColor(sf::Color(255, 69, 0, 150));
        }

        mZones.push_back(zone);
    }
}

void TowerMenu::initializeLabels()
{
    mUpgradeLabels.clear();
    for (int i = 0; i < mNumberOfUpgrades; ++i) {
        sf::Text label;
        label.setFont(mFont);
        label.setCharacterSize(11);
        label.setFillColor(sf::Color::White);

        if (i == 0)
            label.setString("Damage");
        else if (i == 1)
            label.setString("Sell");
        else if (i == 2)
            label.setString("Fire Rate");

        float angle = i * 2.0f * PI / mNumberOfUpgrades + PI / mNumberOfUpgrades;
        sf::Vector2f offset(std::cos(angle) * (80.0f * 0.5f), std::sin(angle) * (80.0f * 0.5f));
        if (i == 2) {
            label.setPosition(mHeptagon.getPosition() + offset - sf::Vector2f(41, 13));
        }
        else if(i == 1){
            label.setPosition(mHeptagon.getPosition() + offset - sf::Vector2f(20, 0));
        }
        else {
            label.setPosition(mHeptagon.getPosition() + offset - sf::Vector2f(17, 0));
        }

        mUpgradeLabels.push_back(label);
    }
}

void TowerMenu::initializeStats() {
    mStatsRectangle.setSize(sf::Vector2f(430, 70));
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

    mSellPriceText.setFont(mFont);
    mSellPriceText.setCharacterSize(12);
    mSellPriceText.setFillColor(sf::Color::White);

    mIncrementText.setFont(mFont);
    mIncrementText.setCharacterSize(12);
    mIncrementText.setFillColor(sf::Color::White);

    mDecrementText.setFont(mFont);
    mDecrementText.setCharacterSize(18);
    mDecrementText.setFillColor(sf::Color::White);
    mIncrementVisible = false;
}

void TowerMenu::formatStats() {
    std::ostringstream damageStream;
    damageStream.precision(2);
    damageStream << std::fixed << mCurrentTower->getDamage();
    mDamageText.setString("Damage " + damageStream.str());

    std::ostringstream fireRateStream;
    fireRateStream.precision(2);
    fireRateStream << std::fixed << mCurrentTower->getFireRate();
    mFireRateText.setString("Fire Rate " + fireRateStream.str());

    std::ostringstream rangeStream;
    rangeStream.precision(2);
    float range = mCurrentTower->getRange();
    rangeStream << std::fixed << range;
    mRangeText.setString("Range " + rangeStream.str());

    std::ostringstream sellPriceStream;
    sellPriceStream.precision(2);
    sellPriceStream << std::fixed << mCurrentTower->getSellPrice();
    mSellPriceText.setString("Sell Price " + sellPriceStream.str());
}

void TowerMenu::update(const sf::Vector2f& mousePos, int& crystals, const sf::Vector2f& position, const sf::FloatRect& size) {
    if (!mVisible) return;
    updateHoverEffect(mousePos, crystals, position, size);
}

void TowerMenu::render(sf::RenderWindow& window) {
    if (!mVisible || !mCurrentTower) return;

    window.draw(mRangeCircle);
    window.draw(mHeptagon);
    for (const auto& zone : mZones) {
        window.draw(zone);
    }
    for (const auto& label : mUpgradeLabels)
        window.draw(label);
    drawLines(window);

    window.draw(mStatsRectangle);
    window.draw(mDamageText);
    window.draw(mFireRateText);
    window.draw(mRangeText);
    window.draw(mSellPriceText);
    window.draw(mTowerIcon);

    if (mIncrementVisible) {
        window.draw(mIncrementText);
        window.draw(mDecrementText);
    }
}

bool TowerMenu::containsUpgradeZone(const sf::Vector2f& point) const
{
    return findUpgradeZone(point) != -1;
}

void TowerMenu::handleUpgradeClick(const sf::Vector2f& mousePos, Tower& tower, int& crystals) {
    int zoneIndex = findUpgradeZone(mousePos);
    if (zoneIndex != -1 && zoneIndex != 1) {
        tower.upgrade(zoneIndex, crystals);
        formatStats();
        mIncrementVisible = false;
    }
}

int TowerMenu::findUpgradeZone(const sf::Vector2f& point) const {
    for (unsigned int i = 0; i < mZones.size(); ++i) {
        if (pointInZone(point, mZones[i])) {
            return i;
        }
    }
    return -1;
}

bool TowerMenu::pointInZone(const sf::Vector2f& point, const sf::ConvexShape& zone) const {
    sf::Vector2f A = zone.getPoint(0);
    sf::Vector2f B = zone.getPoint(1);
    sf::Vector2f C = zone.getPoint(2);

    // Compute vectors
    sf::Vector2f v0 = C - A;
    sf::Vector2f v1 = B - A;
    sf::Vector2f v2 = point - A;

    // Compute dot products
    float dot00 = v0.x * v0.x + v0.y * v0.y;
    float dot01 = v0.x * v1.x + v0.y * v1.y;
    float dot02 = v0.x * v2.x + v0.y * v2.y;
    float dot11 = v1.x * v1.x + v1.y * v1.y;
    float dot12 = v1.x * v2.x + v1.y * v2.y;

    // Compute barycentric coordinates
    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return (u >= 0) && (v >= 0) && (u + v < 1);
}

void TowerMenu::drawLines(sf::RenderWindow& window) {
    sf::VertexArray lines(sf::Lines, 14);
    sf::Vector2f center = mHeptagon.getPosition();

    for (int i = 0; i < 7; ++i) {
        lines[i * 2].position = center;
        lines[i * 2 + 1].position = mHeptagon.getTransform().transformPoint(mHeptagon.getPoint(i));
        lines[i * 2].color = sf::Color::White;
        lines[i * 2 + 1].color = sf::Color::White;
    }
    window.draw(lines);
}

void TowerMenu::updateHoverEffect(const sf::Vector2f& mousePos, int& crystals, const sf::Vector2f& position, const sf::FloatRect& size) {
    if (!mCurrentTower) return;

    int zoneIndex = findUpgradeZone(mousePos);
    mIncrementVisible = false;
    mIncrementString = "";
    mDecrementString = "";

    for (unsigned int i = 0; i < mZones.size(); ++i) {
        if (i == zoneIndex) {
            if (i != 1) {
                if (mCurrentTower && mCurrentTower->canUpgrade(i) && mCurrentTower->getUpgrade(i).getCost() <= crystals) {
                    mZones[i].setFillColor(sf::Color(144, 238, 144, 200));

                    float increment = mCurrentTower->getUpgrade(i).getIncrement();
                    int decrement = mCurrentTower->getUpgrade(i).getCost();
                    mIncrementVisible = true;

                    if (i == 0) {
                        mIncrementPosition = mDamageText.getPosition() + sf::Vector2f(mDamageText.getLocalBounds().width + 9, 0);
                        std::ostringstream damageStream;
                        damageStream.precision(2);
                        damageStream << std::fixed << increment;
                        mIncrementString = "+" + damageStream.str();
                    }
                    else if (i == 2) {
                        mIncrementPosition = mFireRateText.getPosition() + sf::Vector2f(mFireRateText.getLocalBounds().width + 9, 0);
                        std::ostringstream fireRateStream;
                        fireRateStream.precision(2);
                        fireRateStream << std::fixed << increment;
                        mIncrementString = "-" + fireRateStream.str();
                    }
                    mDecrementPosition = position + sf::Vector2f(size.width + 9, 0);
                    mDecrementString = "-" + std::to_string(decrement);
                }
                else {
                    mZones[i].setFillColor(sf::Color(192, 192, 192, 200));
                }
            }
            else {
                mZones[i].setFillColor(sf::Color(255, 165, 0, 200));
                int sellValue = mCurrentTower->getSellPrice();
                mDecrementString = "+" + std::to_string(sellValue);
                mIncrementVisible = true;
                mDecrementPosition = position + sf::Vector2f(size.width + 9, 0);
            }
        }
        else {
            if (i != 1) {
                if (mCurrentTower && mCurrentTower->canUpgrade(i) && mCurrentTower->getUpgrade(i).getCost() <= crystals) {
                    mZones[i].setFillColor(sf::Color(0, 255, 0, 150));
                }
                else {
                    mZones[i].setFillColor(sf::Color(169, 169, 169, 150));
                }
            }
            else {
                mZones[i].setFillColor(sf::Color(255, 69, 0, 150));
            }
        }
    }

    if (mIncrementVisible) {
        mIncrementText.setString(mIncrementString);
        mIncrementText.setPosition(mIncrementPosition);
        mDecrementText.setString(mDecrementString);
        mDecrementText.setPosition(mDecrementPosition);
    }
}

void TowerMenu::handleSellClick(Tower& tower, int& crystals) {
    crystals += tower.sell();
    std::cout << "Tower sold!" << std::endl;
    mIncrementVisible = false;
    hide();
}

void TowerMenu::showStats(const Tower& tower) {
    mCurrentTower = &tower;
    mStatsRectangle.setPosition(mStatsPosition);
    formatStats();

    mDamageText.setPosition(mStatsRectangle.getPosition() + sf::Vector2f(115, 20));
    mFireRateText.setPosition(mStatsRectangle.getPosition() + sf::Vector2f(270, 20));
    mRangeText.setPosition(mStatsRectangle.getPosition() + sf::Vector2f(115, 35));
    mSellPriceText.setPosition(mStatsRectangle.getPosition() + sf::Vector2f(270, 35));
    mTowerIcon.setPosition(mStatsRectangle.getPosition() + sf::Vector2f(25, 20));
}
