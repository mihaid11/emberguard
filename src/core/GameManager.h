#pragma once
#include "../Rpg/gamengine/RPGEngine.h"
#include "../TowerDefense/gamengine/GameEngine.h"
#include "../ui/MainMenu.h"
#include "../Rpg/dialogueSystem/DialogueManager.h"
#include "quests/QuestTypes.h"
#include "quests/QuestManager.h"
#include "NotificationManager.h"
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

    GameEngine& getGame();
    RPGEngine& getGameEngine();
    MainMenu& getMainMenu();
    sf::RenderWindow& getWindow();
    QuestManager& getQuestManager();
    NotificationManager& getNotificationManager();

    void dispatchQuestEvent(const GameEvent& event);

private:
    void update();
    void render();

    sf::RenderWindow mWindow;
    sf::Clock mClock;

    float mMaxFps;
    GameState mCurrentState;
    RPGEngine mRpgEngine;
    GameEngine mTowerDefenseEngine;
    MainMenu mMainMenu;

    DialogueManager mDialogueManager;
    QuestManager mQuestManager;
    NotificationManager mNotificationManager;
};
