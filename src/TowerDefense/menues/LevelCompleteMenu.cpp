#include "LevelCompleteMenu.h"
#include "../gamengine/GameEngine.h"
#include "../../GameManager.h"
#include <iostream>

LevelCompleteMenu::LevelCompleteMenu(sf::RenderWindow& window, GameEngine* game,
    GameManager* gameManager, int level, int crystals)
    : mGame(game), mGameManager(gameManager), mLevel(level), mCrystals(crystals),
    continueButton(sf::Vector2f(0, 0), sf::Vector2f(175, 40), "Continue") {

    mMenuShape.setSize(sf::Vector2f(270,140));
    mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
    mMenuShape.setPosition((window.getSize().x - mMenuShape.getSize().x) / 2,
                           (window.getSize().y - mMenuShape.getSize().y) / 2);

    continueButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 45, mMenuShape.getPosition().y + 44));

    continueButton.setCallback([&]() {
        if (mGameManager)
            mGameManager->switchToRPG(mCrystals);
        else
            std::cerr << "Error: GameManager is nullptr in continueButton callback." << std::endl;
    });

    mButtons.push_back(continueButton);
}

void LevelCompleteMenu::render(sf::RenderWindow& window) {
    window.draw(mMenuShape);

    for (auto& button : mButtons)
        button.render(window);
}

void LevelCompleteMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    for (auto& button : mButtons) {
        if (button.isMouseOver(mousePos))
            button.onClick();
    }
}

void LevelCompleteMenu::updateHover(const sf::Vector2f& mousePos) {
    for (auto& button : mButtons)
        button.updateHover(mousePos);
}

void LevelCompleteMenu::updateCrystals(int crystals) {
    mCrystals = crystals;
}

