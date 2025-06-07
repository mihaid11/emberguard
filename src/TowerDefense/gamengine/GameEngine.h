#pragma once
#include <SFML/Graphics.hpp>
#include "../waves/Map.h"
#include "../enemy/Enemy.h"
#include "../tower/Tower.h"
#include "../waves/Wave.h"
#include "../projectiles/Projectile.h"
#include "../tower/TowerSelectionMenu.h"
#include "../tower/TowerMenu.h"
#include "../tower/LaserTower.h"
#include "../tower/FlameTurret.h"
#include "../player/Player.h"
#include "../player/PlayerMenu.h"
#include "../menues/SmallMenu.h"
#include "../menues/GameOverMenu.h"
#include "../menues/LevelCompleteMenu.h"
#include "../../TimeSystem.h"

class GameManager;

class GameEngine {
public:
    GameEngine(sf::RenderWindow& window, GameManager* gameManager);
    ~GameEngine();

    void processEvents();
    void update();
    void render();

    bool isGameOver() const;
    void init(int level, int crytals, const std::vector<int>& availableTowers);
    bool isLevelCompleted(int level) const;

private:

    void handleTowerClick(Tower& tower, const sf::Vector2f& worldPos);
    void handleNonTowerClick(const sf::Vector2f& worldPos);
    void handleKeyPress(sf::Keyboard::Key keycode);

    void updateButtonHover(sf::RectangleShape& button, sf::Text& buttonText, const sf::Vector2f& mousePos);

    sf::RenderWindow& mWindow;
    Map mMap;
    sf::Clock mClock;
    float mSpawnTimer;

    std::vector<Enemy> mEnemies;
    std::vector<Tower*> mTowers;
    std::vector<Projectile> mProjectiles;

    float mTowerHealth;
    sf::RectangleShape mHealthBar;
    sf::RectangleShape mHealthBarBackground;

    Wave mCurrentWave;
    unsigned int mCurrentWaveNumber;
    unsigned int mCurrentLevel;

    sf::Text mCrystalText;
    sf::Font mFont;
    int mCrystals;
    int mSpentCrystals;

    Tower* mSelectedTower;

    TowerSelectionMenu mTowerSelectionMenu;
    int mSelectedTowerType;
    sf::Vector2f mMenuOpenPosition;
    TowerMenu mTowerMenu;

    Player mPlayer;
    PlayerMenu mPlayerMenu;
    Enemy* mSelectedEnemy;
    std::vector<int> mAvailableTowers;

    sf::RectangleShape startGameButton;
    sf::Text startGameButtonText;
    bool gameStarted;
    bool mGameOver;
    bool mLevelCompleted;

    SmallMenu mSmallMenu;
    GameOverMenu mGameOverMenu;
    LevelCompleteMenu mLevelCompleteMenu;
    bool mIsPaused;
    GameManager* mGameManager;

    bool mShowText1;
    bool mShowText2;
    sf::Text mOutOfRangePlacement;
    sf::Text mOutOfRangeSelection;
    sf::Clock mClockText1;
    sf::Clock mClockText2;

    sf::RectangleShape mBorderUp;
    sf::RectangleShape mBorderDown;
    sf::RectangleShape mBorderLeft;
    sf::RectangleShape mBorderRight;
};

