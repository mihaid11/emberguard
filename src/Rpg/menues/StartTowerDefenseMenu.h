#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include "../../TowerDefense/menues/Button.h"

class GameManager;
class RPGEngine;

class StartTowerDefenseMenu {
public:
    StartTowerDefenseMenu(sf::RenderWindow& window, std::vector<int>& availableTowers,
        RPGEngine* gameEngine, GameManager* gameManager, int level, int crystals);

    void render(sf::RenderWindow& window);
    void handleMouseClick(const sf::Vector2f& mousePos);
    void updateHover(const sf::Vector2f& mousePos);
    void update(int crystals, std::vector<int>& availableTowers);

private:
    sf::RectangleShape mMenuShape;
    sf::RectangleShape mHoveredZoneShape;
    sf::Text mTitle;
    sf::Font mFont;
    Button mStartButton;

    std::vector<sf::RectangleShape> mTowerSlots;
    std::vector<sf::RectangleShape> mSelectingTowerSlots;

    std::vector<int> mSelectedTowers;
    std::vector<int>& mAvailableTowers;

    GameManager* mGameManager;
    RPGEngine* mGameEngine;

    int mLevel;
    int mCrystals;

    // Rendering of the error message variables
    bool mShowText;
    sf::Text mErrorText;
    sf::Clock mClockText;
};

