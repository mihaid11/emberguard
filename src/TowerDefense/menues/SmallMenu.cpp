#include "SmallMenu.h"
#include "../gamengine/GameEngine.h"
#include "../../core/GameManager.h"
#include <iostream>

SmallMenu::SmallMenu(const sf::Vector2f& windowSize, GameEngine* game, GameManager* gameManager,
                     int level, std::vector<int>& availableTowers)
    : Menu(windowSize), mGame(game), mGameManager(gameManager), mAvailableTowers(availableTowers), mLevel(level) {

    initializeLayout();
}

void SmallMenu::render(sf::RenderWindow& window) {
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

void SmallMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    for (auto& button : mButtons) {
        if (button->isMouseOver(mousePos))
            button->onClick();
    }
}

void SmallMenu::updateHover(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    for (auto& button : mButtons)
        button->updateHover(mousePos);
}

void SmallMenu::update(float dt) {
    if (!mIsActive || !mGame)
        return;

    mEnemiesKilledText.setString("Enemies defeated: " + std::to_string(mGame->getEnemiesKilledInRound()));
    mCrystalsEarnedText.setString("Crystals earned: " + std::to_string(mGame->getCrystalsEarnedInRound()));
    mWaveText.setString("Wave: " + std::to_string(mGame->getCurrentWave()) + " / " + std::to_string(mGame->getMaxWaves()));
    mFinalBalanceText.setString("Leave balance: " + std::to_string(mGame->getInitialCrystals() * 6 / 7) + " crystals");
}

void SmallMenu::initializeLayout() {
    initializeTitle("Menu");

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

    sf::Vector2f buttonSize(150.f, 40.f);
    float startX = mMenuShape.getPosition().x + 20.f;
    float height = mEnemiesKilledText.getLocalBounds().height + mCrystalsEarnedText.getLocalBounds().height +
                   mWaveText.getLocalBounds().height + mFinalBalanceText.getLocalBounds().height;
    float gap = (mMenuShape.getSize().y - mHoveredZoneShape.getSize().y - buttonSize.y * 3.5f -
                 mEnemiesKilledText.getLocalBounds().height - mCrystalsEarnedText.getLocalBounds().height -
                 mWaveText.getLocalBounds().height - mFinalBalanceText.getLocalBounds().height) / 3.f;
    float startY = mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y + buttonSize.y - 5.f;

    mEnemiesKilledText.setPosition(sf::Vector2f(startX, startY));
    mWaveText.setPosition(sf::Vector2f(startX, startY + mEnemiesKilledText.getLocalBounds().height + gap));
    mCrystalsEarnedText.setPosition(sf::Vector2f(startX, startY + mEnemiesKilledText.getLocalBounds().height +
                                                         mWaveText.getLocalBounds().height + gap * 2));
    mFinalBalanceText.setPosition(sf::Vector2f(startX, startY + mEnemiesKilledText.getLocalBounds().height + gap * 3 +
                                                       mWaveText.getLocalBounds().height + mCrystalsEarnedText.getLocalBounds().height));

    startY = mMenuShape.getPosition().y + mMenuShape.getSize().y - buttonSize.y * 1.5f;
    gap = (mMenuShape.getSize().x - buttonSize.x * 3.f) / 4.f;
    startX = mMenuShape.getPosition().x + gap;

    auto resumeButton = std::make_unique<Button>(sf::Vector2f(startX, startY), buttonSize, "Resume");
    auto restartButton = std::make_unique<Button>(sf::Vector2f(startX + buttonSize.x + gap, startY), buttonSize, "Restart");
    auto surrenderButton = std::make_unique<Button>(sf::Vector2f(startX + buttonSize.x * 2.f + gap * 2.f, startY), buttonSize, "Surrender");

    resumeButton->setCallback([this]() {
        mIsActive = false;
    });

    restartButton->setCallback([this]() {
        if (mGame)
            mGame->init(mLevel, mGame->getInitialCrystals(), mAvailableTowers);
        else
            std::cerr << "Error: Game is nullptr in restartButton callback." << std::endl;
    });

    surrenderButton->setCallback([this]() {
        if (mGameManager)
            mGameManager->switchToRPG(mGame->getInitialCrystals() * 6 / 7);
        else
            std::cerr << "Error: GameManager is nullptr in returnButton callback." << std::endl;
    });

    mButtons.push_back(std::move(resumeButton));
    mButtons.push_back(std::move(restartButton));
    mButtons.push_back(std::move(surrenderButton));
}
