#include "GameOverMenu.h"
#include "../gamengine/GameEngine.h"
#include "../../GameManager.h"
#include <iostream>

GameOverMenu::GameOverMenu(sf::RenderWindow& window, GameEngine* game, GameManager* gameManager,
    int level, int crystals, std::vector<int>& availableTowers)
    : mGame(game), mLevel(level), mCrystals(crystals), mGameManager(gameManager), mAvailableTowers(availableTowers),
    restartButton(sf::Vector2f(0, 0), sf::Vector2f(175, 40), "Restart"),
    exitButton(sf::Vector2f(0, 0), sf::Vector2f(175, 40), "Exit")
{
    mMenuShape.setSize(sf::Vector2f(270, 140));
    mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
    mMenuShape.setPosition(
        (window.getSize().x - mMenuShape.getSize().x) / 2,
        (window.getSize().y - mMenuShape.getSize().y) / 2
    );

    exitButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 45, mMenuShape.getPosition().y + 77));
    restartButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 45, mMenuShape.getPosition().y + 25));

    restartButton.setCallback([this]() {
        if (mGame) {
            mGame->init(mLevel, mCrystals * 4 /5, mAvailableTowers);
        }
        else {
            std::cerr << "Error: Game is nullptr in restartButton callback." << std::endl;
        }
        });

    exitButton.setCallback([&]() {
        if (mGameManager) {
            mGameManager->switchToRPG(mCrystals * 4 / 5);
        }
        else {
            std::cerr << "Error: GameManager is nullptr in returnButton callback." << std::endl;
        }
        });

    mButtons.push_back(restartButton);
    mButtons.push_back(exitButton);
}

void GameOverMenu::render(sf::RenderWindow& window)
{
    window.draw(mMenuShape);

    for (auto& button : mButtons) {
        button.render(window);
    }
}

void GameOverMenu::handleMouseClick(const sf::Vector2f& mousePos)
{
    for (auto& button : mButtons) {
        if (button.isMouseOver(mousePos)) {
            button.onClick();
        }
    }
}

void GameOverMenu::updateHover(const sf::Vector2f& mousePos)
{
    for (auto& button : mButtons) {
        button.updateHover(mousePos);
    }
}
