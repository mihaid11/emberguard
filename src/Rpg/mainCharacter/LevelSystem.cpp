#include "LevelSystem.h"
#include <iostream>
#include "../inventory/items/TowerBlueprintEpic.h"


LevelSystem::LevelSystem(Inventory* inventory, LevelCompleteMenu* levelCompleteMenu, int maxLevel)
    : mInventory(inventory), mLevelCompleteMenu(levelCompleteMenu), mCurrentXp(0),
    mCurrentLevel(0), mMaxLevel(maxLevel) {

}

void LevelSystem::addXp(int xp) {
    if (mCurrentLevel < mMaxLevel) {
        mCurrentXp += xp;
        checkLevelUp();
    }
}

void LevelSystem::checkLevelUp() {
    while (mCurrentLevel < mMaxLevel && mCurrentXp >= getXpForNextLevel()) {
        mCurrentXp -= getXpForNextLevel();
        mCurrentLevel++;

        grantRewards(mCurrentLevel);
    }
}

void LevelSystem::grantRewards(int level) {
    mLevelCompleteMenu->setLevel(level);
    mLevelCompleteMenu->setActive(true);
}

int LevelSystem::getCurrentXp() const {
    return mCurrentXp;
}

int LevelSystem::getLevel() const {
    return mCurrentLevel;
}

int LevelSystem::getXpForNextLevel() const {
    return static_cast<float>(100.f * std::pow(mCurrentLevel, 1.25f));
}

float LevelSystem::getXpPercentage() const {
    if (mCurrentLevel >= mMaxLevel)
        return 1.f;

    return static_cast<float>(mCurrentXp) / static_cast<float>(getXpForNextLevel());
}
