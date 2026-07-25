#include "GameManager.h"
#include "quests/QuestTypes.h"
#include <iostream>

GameManager::GameManager()
    : mCurrentState(GameState::MainMenu),
    mWindow(sf::VideoMode(1280, 720), "emberguard"),
    mRpgEngine(mWindow, this), mTowerDefenseEngine(mWindow, this),
    mMainMenu(mWindow, this), mMaxFps(60) {

    mWindow.setFramerateLimit(mMaxFps);
    mQuestManager.loadQuestsFromJson("../src/core/quests/tutorial.json");
}

void GameManager::run() {
    // Restart option
    // mRpgEngine.resetSaveGame();

    while (mWindow.isOpen()) {
        update();
        render();
    }
}

void GameManager::update() {
    float dt = mClock.restart().asSeconds();
    mNotificationManager.update(dt);

    switch (mCurrentState) {
        case GameState::MainMenu:
            mMainMenu.processEvents();
            mMainMenu.update();
            break;
        case GameState::RPG:
            mRpgEngine.processEvents();
            mRpgEngine.update(dt);
            break;
        case GameState::TowerDefense:
            mTowerDefenseEngine.processEvents();
            mTowerDefenseEngine.update(dt);
            break;
    }
}

void GameManager::render() {
    mWindow.clear();

    switch (mCurrentState) {
        case GameState::MainMenu:
            mMainMenu.render();
            break;
        case GameState::RPG:
            mRpgEngine.render();
            break;
        case GameState::TowerDefense:
            mTowerDefenseEngine.render();
            break;
    }

    mWindow.setView(mWindow.getDefaultView());
    mNotificationManager.render(mWindow);

    mWindow.display();
}

// Used for transition between TowerDefense and RPG
void GameManager::switchToRPG(int crystals) {
    mRpgEngine.setCrystals(crystals);
    mRpgEngine.enterRPG();
    mCurrentState = GameState::RPG;

    mClock.restart();
}

// Used for transition between MainMenu and RPG
void GameManager::enterRPG(int saveNumber) {
    mRpgEngine.setSaveNumber(saveNumber);
    mRpgEngine.enterRPG();
    mCurrentState = GameState::RPG;

    mClock.restart();
}

void GameManager::switchToTowerDefense(int crystals, int level, const std::vector<int>& availableTowers) {
    mRpgEngine.exitRPG();
    mCurrentState = GameState::TowerDefense;
    mTowerDefenseEngine.init(level, crystals, availableTowers);

    mClock.restart();
}

void GameManager::switchToMainMenu() {
    mRpgEngine.exitRPG();
    mMainMenu.reset();
    mCurrentState = GameState::MainMenu;

    mClock.restart();
}

GameEngine& GameManager::getGame(){
    return mTowerDefenseEngine;
}

RPGEngine& GameManager::getGameEngine() {
    return mRpgEngine;
}

sf::RenderWindow& GameManager::getWindow() {
    return mWindow;
}

QuestManager& GameManager::getQuestManager() {
    return mQuestManager;
}

NotificationManager& GameManager::getNotificationManager() {
    return mNotificationManager;
}

void GameManager::dispatchQuestEvent(const GameEvent& event) {
    std::vector<std::string> completedQuestTitles;
    std::vector<QuestReward> rewards = mQuestManager.onEvent(event, completedQuestTitles);

    for (const auto& title: completedQuestTitles)
        mNotificationManager.addNotification(NotificationManager::Type::QuestCompleted, title);

    for (const auto& reward: rewards) {
        if (reward.type == "start_quest") {
            mQuestManager.startQuest(reward.targetId);

            std::string title = mQuestManager.getQuestTitle(reward.targetId);
            mNotificationManager.addNotification(NotificationManager::Type::QuestStarted, title);
        } else if (reward.type == "crystals" || reward.type == "crystal")
            mRpgEngine.addCrystals(reward.amount);
        else if (reward.type == "item")
            mRpgEngine.rewardItem(reward.targetId, reward.amount);
    }
}
