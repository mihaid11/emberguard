#include "LevelCompleteMenu.h"
#include "../gamengine/GameEngine.h"
#include "../../Rpg/gamengine/RPGEngine.h"
#include "../../core/GameManager.h"
#include "../../Rpg/inventory/items/Wood.h"
#include "../../Rpg/inventory/items/TowerBlueprint.h"
#include "../../Rpg/inventory/items/TowerBlueprintRare.h"
#include "../../Rpg/inventory/items/TowerBlueprintEpic.h"
#include "../../Rpg/inventory/items/TowerBlueprintMythic.h"
#include "RewardSystem.h"
#include "../../core/quests/QuestTypes.h"
#include <iostream>

LevelCompleteMenu::LevelCompleteMenu(const sf::Vector2f& windowSize, GameEngine* towerGame,
    RPGEngine* rpgGame, GameManager* gameManager, int level, bool isTowerLevel)
    : Menu(windowSize, sf::Vector2f(1.f / 5.3f, 1.f / 2.3f)), mTowerGame(towerGame), mRpgGame(rpgGame), mGameManager(gameManager),
    mLevel(level), mIsTowerLevel(isTowerLevel), mContinueButton(sf::Vector2f(0, 0), sf::Vector2f(165, 40), "Continue") {

    if (isTowerLevel)
        mLevelCompleteText.setString("Level completed!");
    else
        mLevelCompleteText.setString("Level up!");
    mLevelCompleteText.setFont(mFont);
    mLevelCompleteText.setCharacterSize(18);
    mLevelCompleteText.setFillColor(sf::Color::White);
    mLevelCompleteText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + (mMenuShape.getSize().x - mLevelCompleteText.getLocalBounds().width) / 2.f,
                                                mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.05f));

    mRewardText.setFont(mFont);
    mRewardText.setCharacterSize(13);
    mRewardText.setFillColor(sf::Color::White);
    
    mQuantityText.setFont(mFont);
    mQuantityText.setCharacterSize(13);
    mQuantityText.setFillColor(sf::Color::White);


    mRewardShape.setSize(sf::Vector2f(mMenuShape.getSize().x * 0.63f, mMenuShape.getSize().y * 0.56f));
    mRewardShape.setFillColor(sf::Color::Transparent);
    mRewardShape.setOutlineColor(sf::Color::White);
    mRewardShape.setOutlineThickness(1.5f);
    mRewardShape.setPosition(sf::Vector2f(mMenuShape.getPosition().x + (mMenuShape.getSize().x - mRewardShape.getSize().x) / 2.f,
                                          mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.18f));

    mContinueButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + (mMenuShape.getSize().x - mContinueButton.getSize().x) / 2.f,
                                             mRewardShape.getPosition().y + mRewardShape.getSize().y * 1.15f));

    refresh();
}

void LevelCompleteMenu::render(sf::RenderWindow& window) {
    if (!mIsActive)
        return;

    window.draw(mBackground);
    window.draw(mMenuShape);

    window.draw(mLevelCompleteText);
    window.draw(mRewardShape);
    window.draw(mRewardText);
    window.draw(mQuantityText);
    window.draw(mItemIcon);

    mContinueButton.render(window);
}

void LevelCompleteMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    if (mContinueButton.isMouseOver(mousePos))
        mContinueButton.onClick();
}

void LevelCompleteMenu::updateHover(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    mContinueButton.updateHover(mousePos);
}

void LevelCompleteMenu::update(float dt) {

}

void LevelCompleteMenu::refresh() {
     if (mIsTowerLevel)
        mReward = RewardSystem::generateTowerLevelReward(mLevel);
    else
        mReward = RewardSystem::generateReward(mLevel);

    mContinueButton.setCallback([this]() {
        if (mIsTowerLevel) {
            if (mGameManager) {
                if (mReward.itemId == 0)
                    mGameManager->switchToRPG(mTowerGame->getCrystals() + mReward.quantity);
                else
                    mGameManager->switchToRPG(mTowerGame->getCrystals());

                int xp = static_cast<int>(50.f * std::pow(mLevel, 1.3f) * 0.6f);
                mGameManager->getGameEngine().addXp(xp);

                mGameManager->getGameEngine().advanceTowerDefenseLevel();
                mGameManager->dispatchQuestEvent(GameEvent{ObjectiveType::complete_level_td, "level_" + std::to_string(mLevel), 1});

                mIsActive = false;
            } else
                std::cerr << "Error: GameManager is nullptr in continueButton callback." << std::endl;
        } else {
            if (mReward.itemId == 0)
                mRpgGame->addCrystals(mReward.quantity);
            mGameManager->dispatchQuestEvent(GameEvent{ObjectiveType::complete_level, "level_" + std::to_string(mLevel), 1});
            mIsActive = false;
        }
    });

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
            mGameManager->getGameEngine().getInventory().addItem(std::move(mItem), mReward.quantity);
        }
    } else {
        mItemIcon.setSize(sf::Vector2f(50.f, 50.f));
        mItemIcon.setFillColor(sf::Color(50, 50, 200, 255));
    }

    mRewardText.setString(mReward.name);
    mRewardText.setPosition(sf::Vector2f(mRewardShape.getPosition().x +(mRewardShape.getSize().x - mRewardText.getLocalBounds().width) / 2.f,
                                         mRewardShape.getPosition().y + mRewardShape.getSize().y * 0.75f - mRewardText.getLocalBounds().height));

    mQuantityText.setString(std::to_string(mReward.quantity));
    mQuantityText.setPosition(sf::Vector2f(mRewardShape.getPosition().x +(mRewardShape.getSize().x - mQuantityText.getLocalBounds().width) / 2.f,
                                           mRewardShape.getPosition().y + mRewardShape.getSize().y * 0.98f - mRewardText.getLocalBounds().height - mQuantityText.getLocalBounds().height));

    mItemIcon.setPosition(sf::Vector2f(mRewardShape.getPosition().x + (mRewardShape.getSize().x - mItemIcon.getSize().x) / 2.f,
                                       mRewardShape.getPosition().y + mRewardShape.getSize().y * 0.2f));
}

int LevelCompleteMenu::getLevel() const {
    return mLevel;
}

void LevelCompleteMenu::setLevel(int level) {
    mLevel = level;
}
