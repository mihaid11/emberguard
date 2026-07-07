#pragma once
#include <SFML/Graphics.hpp>
#include "RewardSystem.h"
#include "../../ui/Menu.h"
#include "../../ui/Button.h"
#include <vector>

class GameEngine;
class RPGEngine;
class GameManager;

// Used for both tower defense level completion and main character level progression
class LevelCompleteMenu : public Menu {
public:
    LevelCompleteMenu(const sf::Vector2f& windowSize, GameEngine* towerGame, RPGEngine* rpgGame,
                      GameManager* gameManager, int level, bool isTowerLevel);

    void render(sf::RenderWindow& window) override;
    void handleMouseClick(const sf::Vector2f& mousePos) override;
    void updateHover(const sf::Vector2f& mousePos) override;
    void update(float dt) override;

    int getLevel() const;
    void setLevel(int level);
    void refresh();

private:
    Button mContinueButton;

    Reward mReward;
    sf::Font mFont;
    sf::Text mRewardText;
    sf::Text mQuantityText;
    sf::Text mLevelCompleteText;
    sf::RectangleShape mItemIcon;
    sf::RectangleShape mRewardShape;

    GameEngine* mTowerGame;
    RPGEngine* mRpgGame;
    GameManager* mGameManager;
    int mLevel;
    bool mIsTowerLevel;
};
