#include "StartTowerDefenseMenu.h"
#include <iostream>
#include "../../GameManager.h"
#include "../gamengine/RPGEngine.h"

StartTowerDefenseMenu::StartTowerDefenseMenu(sf::RenderWindow& window, std::vector<int>& availableTowers,
                                             RPGEngine* gameEngine, GameManager* gameManager, int level, int crystals)
    : mStartButton(sf::Vector2f(826.f, 570.f), sf::Vector2f(80.0f, 30.0f), "Start"),
    mGameManager(gameManager), mAvailableTowers(availableTowers), mCrystals(crystals),
    mLevel(level), mGameEngine(gameEngine), mShowText(false) {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Couldn't load font from file" << std::endl;

    mMenuShape.setSize(sf::Vector2f(window.getSize().x * 3.0f / 4.0f, window.getSize().y * 3.0f / 4.0f));
    mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
    mMenuShape.setPosition(sf::Vector2f((window.getSize().x - mMenuShape.getSize().x) / 2.0f,
                                        (window.getSize().y - mMenuShape.getSize().y) / 2.0f));

    mHoveredZoneShape.setSize(sf::Vector2f(window.getSize().x * 3.0f / 4.0f, 58));
    mHoveredZoneShape.setFillColor(sf::Color(10, 10, 10, 100));
    mHoveredZoneShape.setPosition(sf::Vector2f((window.getSize().x - mMenuShape.getSize().x) / 2.0f,
                                               (window.getSize().y - mMenuShape.getSize().y) / 2.0f));

    mMinimapBorder.setSize(sf::Vector2f(190.f, 110.f));
    mMinimapBorder.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.58f,
                                            mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.48f));
    mMinimapBorder.setFillColor(sf::Color::Black);
    mMinimapBorder.setOutlineColor(sf::Color::White);
    mMinimapBorder.setOutlineThickness(1.7f);

    const auto& paths = getPaths()[level - 1];
    const float scaleX = mMinimapBorder.getSize().x / window.getSize().x;
    const float scaleY = mMinimapBorder.getSize().y / window.getSize().y;

    for (const auto& path : paths) {
        for (size_t i = 0; i < path.size() - 1; ++i) {
            sf::RectangleShape segment;
            segment.setFillColor(sf::Color(128, 128, 128));

            sf::Vector2f start(path[i].x * scaleX + mMinimapBorder.getPosition().x, path[i].y * scaleY + mMinimapBorder.getPosition().y);
            sf::Vector2f end(path[i + 1].x * scaleX + mMinimapBorder.getPosition().x, path[i + 1].y * scaleY + mMinimapBorder.getPosition().y);

            if (start.x == end.x) {
                // Vertical path
                segment.setSize({50.f * scaleX, std::abs(end.y - start.y)});
                segment.setPosition(start.x - 25.f * scaleX, std::min(start.y, end.y));
            } else {
                // Horizontal path
                segment.setSize({std::abs(end.x - start.x), 50.f * scaleY});
                segment.setPosition(std::min(start.x, end.x), start.y - 25.f * scaleY);
            }
            mMinimapPaths.push_back(segment);
        }
    }

    mDifficultyText.setFont(mFont);
    mDifficultyText.setCharacterSize(16);
    mDifficultyText.setPosition(sf::Vector2f(mMinimapBorder.getPosition().x + mMinimapBorder.getSize().x + 34.f,
                                             mMinimapBorder.getPosition().y - 5.f));
    mDifficultyText.setFillColor(sf::Color::White);
    mDifficultyText.setString("Difficuly");

    int currentDifficulty;
    if (level == 1)
        currentDifficulty = 2;
    else
        currentDifficulty = 3;

    float circleRadius = 8.f;
    const sf::Vector2f startPos(mDifficultyText.getPosition().x, mDifficultyText.getPosition().y + 28.5f);
    for (int i = 0; i < 5; ++i) {
        sf::CircleShape circle(circleRadius);
        circle.setPosition(startPos.x + i * (circleRadius * 2 + 6.35f), startPos.y);
        circle.setFillColor(i < currentDifficulty ? sf::Color::White : sf::Color(100, 100, 100));
        mDifficultyCircles.push_back(circle);
    }

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
    mTitle.setPosition(sf::Vector2f(mMenuShape.getPosition().x + (mMenuShape.getSize().x - mTitle.getScale().x) / 2.2f,
                                    mMenuShape.getPosition().y + 15.f));

    mErrorText.setFillColor(sf::Color::White);
    mErrorText.setFont(mFont);
    mErrorText.setCharacterSize(18);
    mErrorText.setString("Select at least one tower!");
    mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2.f +
                                       (mMenuShape.getSize().x / 2.f - mErrorText.getLocalBounds().width) / 2.f - 5.f,
                                        mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.77f));

    mCurrentLevelText.setFillColor(sf::Color::White);
    mCurrentLevelText.setFont(mFont);
    mCurrentLevelText.setCharacterSize(31);
    std::string levelString = "Level " + std::to_string(level);
    mCurrentLevelText.setString(levelString);
    mCurrentLevelText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.75f -
                                               mCurrentLevelText.getLocalBounds().width / 2.f,
                                               mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.15f));

    mLevelDescriptionText.setFillColor(sf::Color::White);
    mLevelDescriptionText.setFont(mFont);
    mLevelDescriptionText.setCharacterSize(15);

    std::string descriptionString = "";
    if(level == 1)
        descriptionString = "There once was a ship that put to sea\nThe name of the ship was the Billy O Tea";

    mLevelDescriptionText.setString(descriptionString);
    mLevelDescriptionText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.585f,
                                                   mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.3f));

    mLine.setFillColor(sf::Color(10, 10, 10, 100));
    mLine.setSize(sf::Vector2f(2.9f, mMenuShape.getSize().y - mHoveredZoneShape.getSize().y));
    mLine.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2.f - mLine.getSize().x / 2.f,
                                   mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y));

    mStartButton.setCallback([&]() {
        if(!mSelectedTowers.empty()) {
            mGameEngine->saveGame();
            mGameManager->switchToTowerDefense(mCrystals, mLevel, mSelectedTowers);
        } else {
            mErrorText.setFillColor(sf::Color::White);
            mClock.restart();
            mShowText = true;
        }
    });
}

void StartTowerDefenseMenu::render(sf::RenderWindow& window) {
    window.draw(mMenuShape);
    window.draw(mHoveredZoneShape);
    window.draw(mLevelDescriptionText);
    window.draw(mCurrentLevelText);
    window.draw(mLine);

    // Minimap rendering
    window.draw(mMinimapBorder);
    for (const auto& path: mMinimapPaths)
        window.draw(path);

    // Difficulty rendering
    window.draw(mDifficultyText);
    for (auto& circle: mDifficultyCircles)
        window.draw(circle);

    for (int i = 0; i < mTowerSlots.size(); ++i) {
        if (i < mSelectedTowers.size()) {
            if (mSelectedTowers[i] == 1)
                mTowerSlots[i].setFillColor(sf::Color::Green);
            else if (mSelectedTowers[i] == 2)
                mTowerSlots[i].setFillColor(sf::Color::Red);
        } else {
            mTowerSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        }
        window.draw(mTowerSlots[i]);
    }

    for (int i = 0; i < mSelectingTowerSlots.size(); ++i) {
        if (i < mAvailableTowers.size()) {
            if (mAvailableTowers[i] == 1)
                mSelectingTowerSlots[i].setFillColor(sf::Color::Green);
            else if (mAvailableTowers[i] == 2)
                mSelectingTowerSlots[i].setFillColor(sf::Color::Red);
        } else {
            mSelectingTowerSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        }
        window.draw(mSelectingTowerSlots[i]);
    }

    window.draw(mTitle);
    mStartButton.render(window);

    // If the error text is visible gradually make it dissapear and render it
    if (mShowText) {
        float elapsedTime = mClock.getElapsedTime().asSeconds();
        if (elapsedTime > 1.8f) {
            mShowText = false;
            mErrorText.setString("");
        } else {
            int alpha = static_cast<int>(255 * (1.0f - (elapsedTime / 1.8f)));
            mErrorText.setFillColor(sf::Color(255, 255, 255, alpha));
        }
        window.draw(mErrorText);
    }
}

void StartTowerDefenseMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    for (int i = 0; i < mSelectingTowerSlots.size(); ++i) {
        if (mSelectingTowerSlots[i].getGlobalBounds().contains(mousePos)) {
            if (mSelectedTowers.size() < mTowerSlots.size()) {
                int id = mAvailableTowers[i];
                mSelectedTowers.push_back(id);
                mAvailableTowers.erase(mAvailableTowers.begin() + i);
                update(mCrystals, mAvailableTowers);
            }
            return;
        }
    }

    for (int i = 0; i < mTowerSlots.size(); ++i) {
        if (mTowerSlots[i].getGlobalBounds().contains(mousePos) && i < mSelectedTowers.size()) {
            int id = mSelectedTowers[i];
            mAvailableTowers.push_back(id);
            mSelectedTowers.erase(mSelectedTowers.begin() + i);
            update(mCrystals, mAvailableTowers);
        }
    }

    if (mStartButton.isMouseOver(mousePos))
        mStartButton.onClick();
}

void StartTowerDefenseMenu::updateHover(const sf::Vector2f& mousePos) {
    mStartButton.updateHover(mousePos);
}

void StartTowerDefenseMenu::update(int crystals, std::vector<int>& availableTowers) {
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
        } else {
            mErrorText.setString("Select at least one tower!");
            mErrorText.setFillColor(sf::Color::White);
            mClock.restart();
            mShowText = true;
        }
    });
}

