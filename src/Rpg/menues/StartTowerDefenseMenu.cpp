#include "StartTowerDefenseMenu.h"
#include <iostream>
#include "../../GameManager.h"
#include "../gamengine/RPGEngine.h"

StartTowerDefenseMenu::StartTowerDefenseMenu(sf::RenderWindow& window, std::vector<int>& availableTowers,
    RPGEngine* gameEngine, GameManager* gameManager, int level, int crystals)
    : mStartButton(sf::Vector2f(1000.0f, 575.0f), sf::Vector2f(80.0f, 30.0f), "Start"),
    mGameManager(gameManager), mAvailableTowers(availableTowers), mCrystals(crystals),
    mLevel(level), mGameEngine(gameEngine), mShowText(false)
{
    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf")) {
        std::cout << "Couldn't load font from file" << std::endl;
    }

    mMenuShape.setSize(sf::Vector2f(window.getSize().x * 3.0f / 4.0f, window.getSize().y * 3.0f / 4.0f));
    mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
    mMenuShape.setPosition(sf::Vector2f((window.getSize().x - mMenuShape.getSize().x) / 2.0f,
        (window.getSize().y - mMenuShape.getSize().y) / 2.0f));

    mHoveredZoneShape.setSize(sf::Vector2f(window.getSize().x * 3.0f / 4.0f, 58));
    mHoveredZoneShape.setFillColor(sf::Color(10, 10, 10, 100));
    mHoveredZoneShape.setPosition(sf::Vector2f((window.getSize().x - mMenuShape.getSize().x) / 2.0f,
        (window.getSize().y - mMenuShape.getSize().y) / 2.0f));

    mTowerSlots.resize(6);
    sf::Vector2f position(mMenuShape.getPosition().x + 50.f, mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y + 50.f);
    for (int i = 0; i < 6; ++i) {
        mTowerSlots[i].setSize(sf::Vector2f(50.f, 50.f));
        mTowerSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        mTowerSlots[i].setOutlineColor(sf::Color::White);
        mTowerSlots[i].setOutlineThickness(2.0f);
        mTowerSlots[i].setPosition(position.x + i * 50.f, position.y);
    }
    mSelectedTowers.reserve(mTowerSlots.size());
    mSelectingTowerSlots.resize(availableTowers.size());
    sf::Vector2f position1(mMenuShape.getPosition().x + 50.f, mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y + 150.f);
    int indent = 0;
    for (int i = 0; i < availableTowers.size(); ++i) {
        mSelectingTowerSlots[i].setSize(sf::Vector2f(50.f, 50.f));
        mSelectingTowerSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        mSelectingTowerSlots[i].setOutlineColor(sf::Color::White);
        mSelectingTowerSlots[i].setOutlineThickness(2.0f);
        if (i % 6 == 0 && i)
            indent += 60;
        mSelectingTowerSlots[i].setPosition(position1.x + i * 50.f, position1.y + indent);
    }

    mTitle.setFillColor(sf::Color::White);
    mTitle.setFont(mFont);
    mTitle.setCharacterSize(20);
    mTitle.setString("Tower Defense");
    mTitle.setPosition(sf::Vector2f(mMenuShape.getPosition().x + (mMenuShape.getSize().x - mTitle.getScale().x) / 2.2f
        , mMenuShape.getPosition().y + 15.f));

    mErrorText.setFillColor(sf::Color::White);
    mErrorText.setFont(mFont);
    mErrorText.setCharacterSize(18);
    mErrorText.setString("Select at least one tower!");
    mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.7f,
                                        mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.8f));

    mStartButton.setCallback([&]() {
        if(!mSelectedTowers.empty()) {
            mGameEngine->saveGame();
            mGameManager->switchToTowerDefense(mCrystals, mLevel, mSelectedTowers);
        }
        else {
            mErrorText.setFillColor(sf::Color::White);
            mClockText.restart();
            mShowText = true;
        }
        });
}

void StartTowerDefenseMenu::render(sf::RenderWindow& window)
{
    window.draw(mMenuShape);
    window.draw(mHoveredZoneShape);
    for (int i = 0; i < mTowerSlots.size(); ++i)
    {
        if (i < mSelectedTowers.size())
        {
            if (mSelectedTowers[i] == 1)
                mTowerSlots[i].setFillColor(sf::Color::Green);
            else if (mSelectedTowers[i] == 2)
                mTowerSlots[i].setFillColor(sf::Color::Red);
        }
        else
            mTowerSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        window.draw(mTowerSlots[i]);
    }

    for (int i = 0; i < mSelectingTowerSlots.size(); ++i)
    {
        if (i < mAvailableTowers.size())
        {
            if (mAvailableTowers[i] == 1)
                mSelectingTowerSlots[i].setFillColor(sf::Color::Green);
            else if (mAvailableTowers[i] == 2)
                mSelectingTowerSlots[i].setFillColor(sf::Color::Red);
        }
        else
            mSelectingTowerSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        window.draw(mSelectingTowerSlots[i]);
    }

    window.draw(mTitle);
    mStartButton.render(window);

    // If the error text is visible gradually make it dissapear and render it
    if (mShowText) {
        float elapsedTime = mClockText.getElapsedTime().asSeconds();
        if (elapsedTime > 1.8f) {
            mShowText = false;
            mErrorText.setString("");
        }
        else {
            int alpha = static_cast<int>(255 * (1.0f - (elapsedTime / 1.8f)));
            mErrorText.setFillColor(sf::Color(255, 255, 255, alpha));
        }
        window.draw(mErrorText);
    }
}

void StartTowerDefenseMenu::handleMouseClick(const sf::Vector2f& mousePos)
{
    for (int i = 0; i < mSelectingTowerSlots.size(); ++i)
    {
        if (mSelectingTowerSlots[i].getGlobalBounds().contains(mousePos))
        {
            if (mSelectedTowers.size() < mTowerSlots.size())
            {
                int id = mAvailableTowers[i];
                mSelectedTowers.push_back(id);
                mAvailableTowers.erase(mAvailableTowers.begin() + i);
                update(mCrystals, mAvailableTowers);
            }
            return;
        }
    }

    for (int i = 0; i < mTowerSlots.size(); ++i)
    {
        if (mTowerSlots[i].getGlobalBounds().contains(mousePos) && i < mSelectedTowers.size())
        {
            int id = mSelectedTowers[i];
            mAvailableTowers.push_back(id);
            mSelectedTowers.erase(mSelectedTowers.begin() + i);
            update(mCrystals, mAvailableTowers);
        }
    }

    if (mStartButton.isMouseOver(mousePos))
        mStartButton.onClick();
}

void StartTowerDefenseMenu::updateHover(const sf::Vector2f& mousePos)
{
    mStartButton.updateHover(mousePos);
}

void StartTowerDefenseMenu::update(int crystals, std::vector<int>& availableTowers)
{
    mCrystals = crystals;
    mAvailableTowers = availableTowers;

    // Updating tower slots
    mSelectingTowerSlots.resize(availableTowers.size());
    sf::Vector2f position1(mMenuShape.getPosition().x + 50.f, mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y + 150.f);
    int indent = 0;
    for (int i = 0; i < availableTowers.size(); ++i) {
        mSelectingTowerSlots[i].setSize(sf::Vector2f(50.f, 50.f));
        mSelectingTowerSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        mSelectingTowerSlots[i].setOutlineColor(sf::Color::White);
        mSelectingTowerSlots[i].setOutlineThickness(2.0f);
        if (i % 6 == 0 && i)
            indent += 60;
        mSelectingTowerSlots[i].setPosition(position1.x + i * 50.f, position1.y + indent);
    }

    mStartButton.setCallback([&]() {
        if (!mSelectedTowers.empty()) {
            mGameEngine->saveGame();
            mGameManager->switchToTowerDefense(mCrystals, mLevel, mSelectedTowers);
        }
        else {
            mErrorText.setString("Select at least one tower!");
            mErrorText.setFillColor(sf::Color::White);
            mClockText.restart();
            mShowText = true;
        }
        });
}
