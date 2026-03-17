#pragma once
#include <functional>
#include <math.h>
#include "../inventory/Inventory.h"
#include "../../TowerDefense/menues/LevelCompleteMenu.h"

class LevelSystem {
public:
    LevelSystem(Inventory* inventory, LevelCompleteMenu* levelCompleteMenu, int maxLevel = 50);

    void addXp(int xp);
    
    int getLevel() const;
    void setLevel(int level);

    int getCurrentXp() const;
    int getXpForNextLevel() const;
    float getXpPercentage() const;
    void setXp(int xp);

    void grantRewards(int level);

private:
    void checkLevelUp();

    int mCurrentLevel;
    int mCurrentXp;
    int mMaxLevel;

    Inventory* mInventory;
    LevelCompleteMenu* mLevelCompleteMenu;
};
