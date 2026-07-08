#include "GameOverMenu.h"
#include "../gamengine/GameEngine.h"
#include "../../core/GameManager.h"
#include <iostream>

GameOverMenu::GameOverMenu(const sf::Vector2f& windowSize, GameEngine* game, GameManager* gameManager,
    int level, int crystals, std::vector<int>& availableTowers)
    : Menu(windowSize), mGame(game), mLevel(level), mCrystals(crystals), mGameManager(gameManager),
    mAvailableTowers(availableTowers), mRestartButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(150.f, 40.f), "Restart"),
    mExitButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(150.f, 40.f), "Exit") {

    initializeLayout();
}

void GameOverMenu::render(sf::RenderWindow& window) {
    if (!mIsActive)
        return;

    Menu::render(window);

    window.draw(mEnemiesKilledText);
    window.draw(mCrystalsEarnedText);
    window.draw(mWaveText);
    window.draw(mFinalBalanceText);

    for (auto& button : mButtons)
        button->render(window);
}

void GameOverMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    for (auto& button : mButtons) {
        if (button->isMouseOver(mousePos))
            button->onClick();
    }
}

void GameOverMenu::updateHover(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    for (auto& button : mButtons)
        button->updateHover(mousePos);
}

void GameOverMenu::update(float dt) {
    if (!mIsActive || !mGame)
        return;

    mEnemiesKilledText.setString("Enemies defeated: " + std::to_string(mGame->getEnemiesKilledInRound()));
    mCrystalsEarnedText.setString("Crystals earned: " + std::to_string(mGame->getCrystalsEarnedInRound()));
    mWaveText.setString("Wave: " + std::to_string(mGame->getCurrentWave()) + " / " + std::to_string(mGame->getMaxWaves()));
    mFinalBalanceText.setString("Leave balance: " + std::to_string(mGame->getInitialCrystals() * 6 / 7) + " crystals");
}

void GameOverMenu::initializeLayout() {
    initializeTitle("Game Over");

    mEnemiesKilledText.setFont(mFont);
    mEnemiesKilledText.setCharacterSize(17);
    mEnemiesKilledText.setFillColor(sf::Color::White);
    mEnemiesKilledText.setString("Enemies defeated: 0");

    mCrystalsEarnedText.setFont(mFont);
    mCrystalsEarnedText.setCharacterSize(17);
    mCrystalsEarnedText.setFillColor(sf::Color::White);
    mCrystalsEarnedText.setString("Crystals earned: 0");

    mWaveText.setFont(mFont);
    mWaveText.setCharacterSize(17);
    mWaveText.setFillColor(sf::Color::White);
    mWaveText.setString("Wave: 0/0");

    mFinalBalanceText.setFont(mFont);
    mFinalBalanceText.setCharacterSize(17);
    mFinalBalanceText.setFillColor(sf::Color::White);
    mFinalBalanceText.setString("Leave balance: 0 crystals");

    float startX = mMenuShape.getPosition().x + 20.f;
    float height = mEnemiesKilledText.getLocalBounds().height + mCrystalsEarnedText.getLocalBounds().height +
                   mWaveText.getLocalBounds().height + mFinalBalanceText.getLocalBounds().height;
    float gap = (mMenuShape.getSize().y - mHoveredZoneShape.getSize().y - mRestartButton.getSize().y * 3.5f -
                 mEnemiesKilledText.getLocalBounds().height - mCrystalsEarnedText.getLocalBounds().height -
                 mWaveText.getLocalBounds().height - mFinalBalanceText.getLocalBounds().height) / 3.f;
    float startY = mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y + mRestartButton.getSize().y - 5.f;

    mEnemiesKilledText.setPosition(sf::Vector2f(startX, startY));
    mWaveText.setPosition(sf::Vector2f(startX, startY + mEnemiesKilledText.getLocalBounds().height + gap));
    mCrystalsEarnedText.setPosition(sf::Vector2f(startX, startY + mEnemiesKilledText.getLocalBounds().height +
                                                         mWaveText.getLocalBounds().height + gap * 2));
    mFinalBalanceText.setPosition(sf::Vector2f(startX, startY + mEnemiesKilledText.getLocalBounds().height + gap * 3 +
                                                       mWaveText.getLocalBounds().height + mCrystalsEarnedText.getLocalBounds().height));

    startY = mMenuShape.getPosition().y + mMenuShape.getSize().y - mRestartButton.getSize().y * 1.5f;
    gap = (mMenuShape.getSize().x - mRestartButton.getSize().x * 2.f) / 3.f;
    startX = mMenuShape.getPosition().x + gap;

    mRestartButton.setPosition(sf::Vector2f(startX, startY));
    mExitButton.setPosition(sf::Vector2f(startX + mRestartButton.getSize().x + gap, startY));

    mRestartButton.setCallback([this]() {
        if (mGame)
            mGame->init(mLevel, mGame->getInitialCrystals(), mAvailableTowers);
        else
            std::cerr << "Error: Game is nullptr in restartButton callback." << std::endl;
    });

    mExitButton.setCallback([this]() {
        if (mGameManager)
            mGameManager->switchToRPG(mGame->getInitialCrystals() * 6 / 7);
        else
            std::cerr << "Error: GameManager is nullptr in returnButton callback." << std::endl;
    });

    mButtons.push_back(&mRestartButton);
    mButtons.push_back(&mExitButton);
}
