#pragma once
#include <SFML/Graphics.hpp>
#include "RewardSystem.h"
#include "Button.h"
#include <vector>

class GameEngine;
class GameManager;

class LevelCompleteMenu {
public:
    LevelCompleteMenu(sf::RenderWindow& window, GameEngine* game,
                      GameManager* gameManager, int level);

    void render(sf::RenderWindow& window);
    void handleMouseClick(const sf::Vector2f& mousePos);
    void updateHover(const sf::Vector2f& mousePos);

private:
    sf::RectangleShape mMenuShape;
    sf::RectangleShape mBackground;

    std::vector<Button> mButtons;
    Button continueButton;

    Reward mReward;
    sf::Text mRewardText;
    sf::Text mQuantityText;
    sf::RectangleShape mItemIcon;
    sf::Font mFont;
    sf::RectangleShape mRewardShape;
    GameEngine* mGame;
    GameManager* mGameManager;
    int mLevel;
};

