#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include "../../ui/Menu.h"
#include "../../ui/Button.h"
#include "../../TowerDefense/waves/PathsConfig.h"

class GameManager;
class RPGEngine;

class StartTowerDefenseMenu : public Menu {
public:
    StartTowerDefenseMenu(const sf::Vector2f& windowSize, std::vector<int>& availableTowers,
                          RPGEngine* gameEngine, GameManager* gameManager, int& level, int& crystals);

    void render(sf::RenderWindow& window) override;
    void handleMouseClick(const sf::Vector2f& mousePos) override;
    void updateHover(const sf::Vector2f& mousePos) override;
    void update(float dt) override;

    void advanceLevel();
    void refresh();

private:
    sf::Vector2f mWindowSize;
    Button mStartButton;

    std::vector<sf::RectangleShape> mTowerSlots;
    std::vector<sf::RectangleShape> mSelectingTowerSlots;

    std::vector<int> mSelectedTowers;
    std::vector<int>& mAvailableTowers;

    GameManager* mGameManager;
    RPGEngine* mGameEngine;

    int& mLevel;
    int& mCrystals;

    // Level information
    sf::Text mCurrentLevelText;
    sf::Text mLevelDescriptionText;
    sf::RectangleShape mLine;

    // Rendering of the error message variables
    bool mShowText;
    sf::Text mErrorText;
    sf::Clock mClock;

    // Minimap variables
    sf::RectangleShape mMinimapBorder;
    std::vector<sf::RectangleShape> mMinimapPaths;

    std::vector<sf::CircleShape> mDifficultyCircles;
    sf::Text mDifficultyText;

    sf::Color getTowerColor(int towerId) const;
};
