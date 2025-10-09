#include "LevelCompleteMenu.h"
#include "../gamengine/GameEngine.h"
#include "../../GameManager.h"
#include "../../Rpg/inventory/items/Wood.h"
#include "../../Rpg/inventory/items/TowerBlueprint.h"
#include "../../Rpg/inventory/items/TowerBlueprintRare.h"
#include "../../Rpg/inventory/items/TowerBlueprintEpic.h"
#include "../../Rpg/inventory/items/TowerBlueprintMythic.h"
#include <iostream>

LevelCompleteMenu::LevelCompleteMenu(sf::RenderWindow& window, GameEngine* game,
    GameManager* gameManager, int level)
    : mGame(game), mGameManager(gameManager), mLevel(level),
    continueButton(sf::Vector2f(0, 0), sf::Vector2f(165, 40), "Continue") {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cerr << "Failed to load font for AnalyzeMenu!" << std::endl;

    mMenuShape.setSize(sf::Vector2f(window.getSize().x / 5.5f, window.getSize().y / 3.f));
    mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
    mMenuShape.setPosition((window.getSize().x - mMenuShape.getSize().x) / 2,
                           (window.getSize().y - mMenuShape.getSize().y) / 2);

    mBackground.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    mBackground.setFillColor(sf::Color(50, 50, 50, 185));
    mBackground.setPosition(sf::Vector2f(0, 0));
    continueButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + (mMenuShape.getSize().x - continueButton.getSize().x) / 2.f,
                                            mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.1f));

    mReward = RewardSystem::generateReward(level);
    continueButton.setCallback([this, game, gameManager]() {
        if (mGameManager) {
            if (mReward.itemId == 0)
                mGameManager->switchToRPG(game->getCrystals() + mReward.quantity);
            else
                mGameManager->switchToRPG(game->getCrystals());
        } else
            std::cerr << "Error: GameManager is nullptr in continueButton callback." << std::endl;
    });

    mButtons.push_back(continueButton);

    mRewardShape.setSize(sf::Vector2f(mMenuShape.getSize().x * 0.6f, mMenuShape.getSize().y * 0.55f));
    mRewardShape.setFillColor(sf::Color::Transparent);
    mRewardShape.setOutlineColor(sf::Color::White);
    mRewardShape.setOutlineThickness(1.5f);
    mRewardShape.setPosition(sf::Vector2f(mMenuShape.getPosition().x + (mMenuShape.getSize().x - mRewardShape.getSize().x) / 2.f,
                                          mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.9f - mRewardShape.getSize().y));

    mRewardText.setFont(mFont);
    mRewardText.setString(mReward.name);
    mRewardText.setCharacterSize(13);
    mRewardText.setFillColor(sf::Color::White);
    mRewardText.setPosition(sf::Vector2f(mRewardShape.getPosition().x +(mRewardShape.getSize().x - mRewardText.getLocalBounds().width) / 2.f,
                                         mRewardShape.getPosition().y + mRewardShape.getSize().y * 0.75f - mRewardText.getLocalBounds().height));

    mQuantityText.setFont(mFont);
    mQuantityText.setString(std::to_string(mReward.quantity));
    mQuantityText.setCharacterSize(13);
    mQuantityText.setFillColor(sf::Color::White);
    mQuantityText.setPosition(sf::Vector2f(mRewardShape.getPosition().x +(mRewardShape.getSize().x - mQuantityText.getLocalBounds().width) / 2.f,
                                         mRewardShape.getPosition().y + mRewardShape.getSize().y * 0.98f - mRewardText.getLocalBounds().height - mQuantityText.getLocalBounds().height));

    if (mReward.itemId) {
        std::unique_ptr<Item> mItem;
        if (mReward.itemId == 2)
            mItem = std::make_unique<TowerBlueprint>();
        else if (mReward.itemId == 3)
            mItem = std::make_unique<TowerBlueprintRare>();
        else if (mReward.itemId == 4)
            mItem = std::make_unique<TowerBlueprintEpic>();
        else if (mReward.itemId == 5)
            mItem = std::make_unique<TowerBlueprintMythic>();
        else if (mReward.itemId == 1)
            mItem = std::make_unique<Wood>();

        if (mItem) {
            mItemIcon = mItem->getIcon();
            gameManager->getGameEngine().getInventory().addItem(std::move(mItem), mReward.quantity);
        }
    } else {
        mItemIcon.setSize(sf::Vector2f(50.f, 50.f));
        mItemIcon.setFillColor(sf::Color(50, 50, 200, 255));
    }
    mItemIcon.setPosition(sf::Vector2f(mRewardShape.getPosition().x + (mRewardShape.getSize().x - mItemIcon.getSize().x) / 2.f,
                                       mRewardShape.getPosition().y + mRewardShape.getSize().y * 0.1f));
}

void LevelCompleteMenu::render(sf::RenderWindow& window) {
    window.draw(mBackground);
    window.draw(mMenuShape);
    window.draw(mRewardShape);
    window.draw(mRewardText);
    window.draw(mQuantityText);
    window.draw(mItemIcon);

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

