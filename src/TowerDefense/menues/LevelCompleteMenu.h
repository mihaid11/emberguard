#pragma once
#include <SFML/Graphics.hpp>
#include "RewardSystem.h"
#include "../../ui/Button.h"
#include <vector>

class GameEngine;
class RPGEngine;
class GameManager;

// Used for both tower defense level completion and main character level progression
class LevelCompleteMenu {
public:
    LevelCompleteMenu(sf::RenderWindow& window, GameEngine* towerGame, RPGEngine* rpgGame,
                      GameManager* gameManager, int level, bool isTowerLevel);

    void render(sf::RenderWindow& window);
    void handleMouseClick(const sf::Vector2f& mousePos);
    void updateHover(const sf::Vector2f& mousePos);

    void refresh();
    bool isActive() const;
    void setActive(bool state);

    int getLevel() const;
    void setLevel(int level);

private:
    sf::RectangleShape mMenuShape;
    sf::RectangleShape mBackground;
    Button mContinueButton;
    bool mActive;

    Reward mReward;
    sf::Text mRewardText;
    sf::Text mQuantityText;
    sf::Text mLevelCompleteText;
    sf::RectangleShape mItemIcon;
    sf::Font mFont;
    sf::RectangleShape mRewardShape;

    GameEngine* mTowerGame;
    RPGEngine* mRpgGame;
    GameManager* mGameManager;
    int mLevel;
    bool mIsTowerLevel;
};

