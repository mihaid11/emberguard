#include "SmallMenu.h"
#include "../gamengine/GameEngine.h"
#include "../../core/GameManager.h"
#include <iostream>

SmallMenu::SmallMenu(const sf::Vector2f& windowSize, GameEngine* game, GameManager* gameManager,
                     int level, std::vector<int>& availableTowers)
    : Menu(windowSize), mGame(game), mGameManager(gameManager), mLevel(level),
    mAvailableTowers(availableTowers), mResumeButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(150.f, 40.f), "Resume"),
    mRestartButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(150.f, 40.f), "Restart"),
    mSurrenderButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(150.f, 40.f), "Surrender") {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cerr << "Failed to load font for SmallMenu!" << std::endl;

    initializeLayout();
}

void SmallMenu::render(sf::RenderWindow& window) {
    if (!mIsActive)
        return;

    Menu::render(window);
    window.draw(mTitle);

    window.draw(mEnemiesKilledText);
    window.draw(mCrystalsEarnedText);
    window.draw(mWaveText);
    window.draw(mFinalBalanceText);

    for (auto& button : mButtons)
        button->render(window);
}

void SmallMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    for (auto& button : mButtons) {
        if (button->isMouseOver(mousePos))
            button->onClick();
    }
}

void SmallMenu::updateHover(const sf::Vector2f& mousePos) {
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
    mTitle.setFont(mFont);
    mTitle.setCharacterSize(19);
    mTitle.setFillColor(sf::Color::White);
    mTitle.setString("Menu");

    mTitle.setOrigin(mTitle.getLocalBounds().left + mTitle.getLocalBounds().width / 2.f,
                     mTitle.getLocalBounds().top + mTitle.getLocalBounds().height / 2.f);
    mTitle.setPosition(sf::Vector2f(mHoveredZoneShape.getPosition().x + mHoveredZoneShape.getSize().x / 2.f,
                                    mHoveredZoneShape.getPosition().y + mHoveredZoneShape.getSize().y / 2.f));

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
    float gap = (mMenuShape.getSize().y - mHoveredZoneShape.getSize().y - mResumeButton.getSize().y * 3.5f -
                 mEnemiesKilledText.getLocalBounds().height - mCrystalsEarnedText.getLocalBounds().height -
                 mWaveText.getLocalBounds().height - mFinalBalanceText.getLocalBounds().height) / 3.f;
    float startY = mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y + mResumeButton.getSize().y - 5.f;

    mEnemiesKilledText.setPosition(sf::Vector2f(startX, startY));
    mWaveText.setPosition(sf::Vector2f(startX, startY + mEnemiesKilledText.getLocalBounds().height + gap));
    mCrystalsEarnedText.setPosition(sf::Vector2f(startX, startY + mEnemiesKilledText.getLocalBounds().height +
                                                         mWaveText.getLocalBounds().height + gap * 2));
    mFinalBalanceText.setPosition(sf::Vector2f(startX, startY + mEnemiesKilledText.getLocalBounds().height + gap * 3 +
                                                       mWaveText.getLocalBounds().height + mCrystalsEarnedText.getLocalBounds().height));

    startY = mMenuShape.getPosition().y + mMenuShape.getSize().y - mResumeButton.getSize().y * 1.5f;
    gap = (mMenuShape.getSize().x - mResumeButton.getSize().x * 3.f) / 4.f;
    startX = mMenuShape.getPosition().x + gap;

    mResumeButton.setPosition(sf::Vector2f(startX, startY));
    mRestartButton.setPosition(sf::Vector2f(startX + mResumeButton.getSize().x + gap, startY));
    mSurrenderButton.setPosition(sf::Vector2f(startX + mResumeButton.getSize().x * 2.f + gap * 2.f, startY));

    mResumeButton.setCallback([this]() {
        mIsActive = false;
    });

    mRestartButton.setCallback([this]() {
        if (mGame)
            mGame->init(mLevel, mGame->getInitialCrystals(), mAvailableTowers);
        else
            std::cerr << "Error: Game is nullptr in restartButton callback." << std::endl;
    });

    mSurrenderButton.setCallback([this]() {
        if (mGameManager)
            mGameManager->switchToRPG(mGame->getInitialCrystals() * 6 / 7);
        else
            std::cerr << "Error: GameManager is nullptr in returnButton callback." << std::endl;
    });

    mButtons.push_back(&mResumeButton);
    mButtons.push_back(&mRestartButton);
    mButtons.push_back(&mSurrenderButton);
}
