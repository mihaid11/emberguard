#include "SmallMenu.h"
#include "../gamengine/GameEngine.h"
#include "../../GameManager.h"
#include <iostream>

SmallMenu::SmallMenu(sf::RenderWindow& window, GameEngine* game, GameManager* gameManager,
                     int level, std::vector<int>& availableTowers)
    : mIsVisible(false), mGame(game), mGameManager(gameManager), mAvailableTowers(availableTowers),
    quitButton(sf::Vector2f(0, 0), sf::Vector2f(175, 40), "Quit"),
    restartButton(sf::Vector2f(0, 0), sf::Vector2f(175, 40), "Restart"), mLevel(level) {

    mMenuBackground.setSize(sf::Vector2f(270, 140));
    mMenuBackground.setFillColor(sf::Color(50, 50, 50, 220));
    mMenuBackground.setPosition((window.getSize().x - mMenuBackground.getSize().x) / 2,
                                (window.getSize().y - mMenuBackground.getSize().y) / 2);

    mBackground.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    mBackground.setFillColor(sf::Color(50, 50, 50, 185));
    mBackground.setPosition(sf::Vector2f(0, 0));

    quitButton.setPosition(sf::Vector2f(mMenuBackground.getPosition().x + 45, mMenuBackground.getPosition().y + 77));
    restartButton.setPosition(sf::Vector2f(mMenuBackground.getPosition().x + 45, mMenuBackground.getPosition().y + 25));

    quitButton.setCallback([this, game, gameManager]() {
        if (mGameManager)
            mGameManager->switchToRPG(game->getInitialCrystals() * 6 / 7);
        else
            std::cerr << "Error: GameManager is nullptr in returnButton callback." << std::endl;
    });

    restartButton.setCallback([this, game]() {
        if (mGame)
            mGame->init(mLevel, game->getInitialCrystals(), mAvailableTowers);
        else
            std::cerr << "Error: Game is nullptr in restartButton callback." << std::endl;
    });


    mButtons.push_back(quitButton);
    mButtons.push_back(restartButton);
}

void SmallMenu::render(sf::RenderWindow& window) {
    if (!mIsVisible) return;

    window.draw(mBackground);
    window.draw(mMenuBackground);
    for (auto& button : mButtons)
        button.render(window);
}

void SmallMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    for (auto& button : mButtons) {
        if (button.isMouseOver(mousePos))
            button.onClick();
    }
}

void SmallMenu::updateHover(const sf::Vector2f& mousePos) {
    for (auto& button : mButtons)
        button.updateHover(mousePos);
}

bool SmallMenu::isVisible() const {
    return mIsVisible;
}

void SmallMenu::show() {
    mIsVisible = true;
}

void SmallMenu::hide() {
    mIsVisible = false;
}

