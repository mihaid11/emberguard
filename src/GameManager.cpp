#include "GameManager.h"
#include <iostream>

GameManager::GameManager() : mCurrentState(GameState::RPG), mWindow(sf::VideoMode(1280, 720), "emberguard"),
    mRpgEngine(mWindow, this), mTowerDefenseEngine(mWindow, this)
{
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
    case GameState::RPG:
        mRpgEngine.processEvents();
        mRpgEngine.update();
        break;
    case GameState::TowerDefense:
        mTowerDefenseEngine.processEvents();
        mTowerDefenseEngine.update();
        break;
    case GameState::Transition:
        updateTransition();
        break;
    }
}

void GameManager::render() {
    switch (mCurrentState) {
    case GameState::RPG:
        mRpgEngine.render();
        break;
    case GameState::TowerDefense:
        mTowerDefenseEngine.render();
        break;
    case GameState::Transition:
        // TODO : Implement transition rendering
        break;
    }
}

void GameManager::startTransition(bool toRPG)
{
    //mCurrentState = GameState::Transition;
    mTransitionPhase = TransitionPhase::FadeOut;
    mTransitionClock.restart();
    mTransitioningToRPG = toRPG;


    mTransitionCircle.setFillColor(sf::Color::Black);
    mTransitionCircle.setRadius(0);
    mTransitionCircle.setOrigin(0, 0);
}

void GameManager::updateTransition()
{
    // TODO : Implement transitioning logic
}



void GameManager::switchToRPG(int crystals) {
    mCurrentState = GameState::RPG;
    mRpgEngine.resume(crystals);
}

void GameManager::switchToTowerDefense(int crystals, int level, const std::vector<int>& availableTowers) {
    mCurrentState = GameState::TowerDefense;
    mTowerDefenseEngine.init(level, crystals, availableTowers);
}

bool GameManager::isLevelCompleted(int level) const
{
    return mTowerDefenseEngine.isLevelCompleted(level);
}

GameEngine& GameManager::getGame()
{
    return mTowerDefenseEngine;
}

RPGEngine& GameManager::getGameEngine()
{
    return mRpgEngine;
}
