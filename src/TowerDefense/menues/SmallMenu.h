#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../../ui/Button.h"
#include "../../ui/Menu.h"

class GameEngine;
class GameManager;

class SmallMenu : public Menu {
public:
    SmallMenu(const sf::Vector2f& windowSize, GameEngine* game, GameManager* gameManager,
              int level, std::vector<int>& availableTowers);

    void render(sf::RenderWindow& window) override;
    void handleMouseClick(const sf::Vector2f& mousePos) override;
    void updateHover(const sf::Vector2f& mousePos) override;
    void update(float dt) override;

private:
    std::vector<Button*> mButtons;
    Button mResumeButton;
    Button mRestartButton;
    Button mSurrenderButton;

    sf::Text mEnemiesKilledText;
    sf::Text mCrystalsEarnedText;
    sf::Text mFinalBalanceText;
    sf::Text mWaveText;

    GameEngine* mGame;
    GameManager* mGameManager;
    int mLevel;

    std::vector<int>& mAvailableTowers;

    void initializeLayout();
};
