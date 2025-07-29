#include "GameManager.h"
#include <iostream>

GameManager::GameManager()
    : mCurrentState(GameState::MainMenu),
    mWindow(sf::VideoMode(1280, 720), "emberguard"),
    mRpgEngine(mWindow, this), mTowerDefenseEngine(mWindow, this),
    mMainMenu(mWindow, this), mMaxFps(60) {

    mWindow.setFramerateLimit(mMaxFps);
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
    switch (mCurrentState) {
        case GameState::MainMenu:
            mMainMenu.processEvents();
            mMainMenu.update();
            break;
        case GameState::RPG:
            mRpgEngine.processEvents();
            mRpgEngine.update();
            break;
        case GameState::TowerDefense:
            mTowerDefenseEngine.processEvents();
            mTowerDefenseEngine.update();
            break;
    }
}

void GameManager::render() {
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
}

// Used for transition between TowerDefense and RPG
void GameManager::switchToRPG(int crystals) {
    mCurrentState = GameState::RPG;
    mRpgEngine.resume(crystals);
}

// Used for transition between MainMenu and RPG
void GameManager::enterRPG(int saveNumber) {
    mRpgEngine.setSaveNumber(saveNumber);
    mRpgEngine.closeMenues();
    mCurrentState = GameState::RPG;
}

void GameManager::switchToTowerDefense(int crystals, int level, const std::vector<int>& availableTowers) {
    mCurrentState = GameState::TowerDefense;
    mTowerDefenseEngine.init(level, crystals, availableTowers);
}

void GameManager::switchToMainMenu() {
    mMainMenu.reset();
    mCurrentState = GameState::MainMenu;
}

bool GameManager::isLevelCompleted(int level) const {
    return mTowerDefenseEngine.isLevelCompleted(level);
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

