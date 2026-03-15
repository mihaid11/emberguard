#include "RewardSystem.h"
#include "../../Rpg/inventory/items/Wood.h"
#include "../../Rpg/inventory/items/TowerBlueprint.h"
#include "../../Rpg/inventory/items/TowerBlueprintRare.h"
#include "../../Rpg/inventory/items/TowerBlueprintEpic.h"
#include "../../Rpg/inventory/items/TowerBlueprintMythic.h"
#include <ctime>

Reward RewardSystem::generateTowerLevelReward(int level) {
    Reward reward;
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int roll = std::rand() % 100;

    if (roll < 50) {
        reward.name = "Crystals";
        reward.itemId = 0;
        reward.quantity = 20 + level * 20;
    } else if (roll < 75) {
        reward.name = "Wood";
        reward.itemId = 1;
        reward.quantity = 5 + level * 2;
    } else if (roll < 90) {
        reward.name = "Tower Blueprint";
        reward.itemId = 2;
        reward.quantity = 1;
    } else if (roll < 97) {
        reward.name = "Rare Tower Blueprint";
        reward.itemId = 3;
        reward.quantity = 1;
    } else {
        reward.name = (roll % 2 == 0) ? "Epic Tower Blueprint" : "Mythic Tower Blueprint";
        reward.itemId = (roll % 2 == 0) ? 4 : 5;
        reward.quantity = 1;
    }

    return reward;
}

Reward RewardSystem::generateReward(int level) {
    Reward reward;

    if (level == 2) {
        reward.name = "Tower Blueprint";
        reward.itemId = 2;
        reward.quantity = 1;
    } else if (level == 3) {
        reward.name = "Crystals";
        reward.itemId = 0;
        reward.quantity = 50;
    } else if (level == 4) {
        reward.name = "Rare Tower Blueprint";
        reward.itemId = 3;
        reward.quantity = 1;
    } else if (level == 5) {
        reward.name = "Epic Tower Blueprint";
        reward.itemId = 4;
        reward.quantity = 1;
    } else if (level == 6) {
        reward.name = "Crystals";
        reward.itemId = 0;
        reward.quantity = 300;
    } else if (level == 7) {
        reward.name = "Rare Tower Blueprint";
        reward.itemId = 3;
        reward.quantity = 3;
    } else if (level == 8) {
        reward.name = "Crystals";
        reward.itemId = 0;
        reward.quantity = 500;
    } else if (level == 9) {
        reward.name = "Epic Tower Blueprint";
        reward.itemId = 4;
        reward.quantity = 1;
    } else if (level == 10) {
        reward.name = "Mythic Tower Blueprint";
        reward.itemId = 5;
        reward.quantity = 1;
    } else {
        reward.name = "Crystals";
        reward.itemId = 0;
        reward.quantity = 1000;
    }

    return reward;
}

