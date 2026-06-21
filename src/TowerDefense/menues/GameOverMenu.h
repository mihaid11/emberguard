#pragma once
#include <SFML/Graphics.hpp>
#include "../../ui/Menu.h"
#include "../../ui/Button.h"
#include <vector>

class GameEngine;
class GameManager;

class GameOverMenu : public Menu {
public:
    GameOverMenu(const sf::Vector2f& windowSize, GameEngine* game, GameManager* gameManager,
                 int level, int crystals, std::vector<int>& availableTowers);

    void render(sf::RenderWindow& window) override;
    void handleMouseClick(const sf::Vector2f& mousePos) override;
    void updateHover(const sf::Vector2f& mousePos) override;
    void update(float dt) override;

private:
    std::vector<Button*> mButtons;
    Button mRestartButton;
    Button mExitButton;

    sf::Font mFont;
    sf::Text mTitle;

    sf::Text mEnemiesKilledText;
    sf::Text mCrystalsEarnedText;
    sf::Text mFinalBalanceText;
    sf::Text mWaveText;

    GameEngine* mGame;
    GameManager* mGameManager;
    int mLevel;

    int mCrystals;
    std::vector<int>& mAvailableTowers;

    void initializeLayout();
};
