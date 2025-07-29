#pragma once
#include "Rpg/gamengine/RPGEngine.h"
#include "TowerDefense/gamengine/GameEngine.h"
#include "MainMenu/MainMenu.h"
#include "Rpg/dialogueSystem/DialogueManager.h"
#include "TimeSystem.h"

enum class GameState {
    MainMenu,
    RPG,
    TowerDefense
};

class GameManager {
public:
    GameManager();
    void run();

    void switchToTowerDefense(int crystals, int level, const std::vector<int>& availableTowers);
    void switchToRPG(int crystals);
    void enterRPG(int saveNumber);
    void switchToMainMenu();
    bool isLevelCompleted(int level) const;

    GameEngine& getGame();
    RPGEngine& getGameEngine();
    MainMenu& getMainMenu();
    sf::RenderWindow& getWindow();

private:
    void update();
    void render();

    sf::RenderWindow mWindow;
    float mMaxFps;
    GameState mCurrentState;
    RPGEngine mRpgEngine;
    GameEngine mTowerDefenseEngine;
    MainMenu mMainMenu;

    DialogueManager mDialogueManager;
};

