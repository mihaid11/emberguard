#pragma once
#include "Rpg/gamengine/RPGEngine.h"
#include "TowerDefense/gamengine/GameEngine.h"
#include "MainMenu/MainMenu.h"
#include "Rpg/dialogueSystem/DialogueManager.h"
#include "TimeSystem.h"

enum class GameState {
    MainMenu,
    RPG,
    TowerDefense,
    Transition
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
    void startTransition(bool toRPG);

private:
    void update();
    void render();
    void updateTransition();

    sf::RenderWindow mWindow;
    float mMaxFps;
    GameState mCurrentState;
    RPGEngine mRpgEngine;
    GameEngine mTowerDefenseEngine;
    MainMenu mMainMenu;

    DialogueManager mDialogueManager;

    // Transition logic
    enum class TransitionPhase {
        FadeOut,
        Wait,
        FadeIn,
        None
    };

    TransitionPhase mTransitionPhase = TransitionPhase::None;
    sf::CircleShape mTransitionCircle;
    sf::Clock mTransitionClock;
    float mTransitionDuration = 1.0f;
    float mWaitDuration = 1.0f;
    bool mTransitioningToRPG = true;
};

