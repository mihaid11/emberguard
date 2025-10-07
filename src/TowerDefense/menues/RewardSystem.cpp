#include "RewardSystem.h"
#include "../../Rpg/inventory/items/Wood.h"
#include "../../Rpg/inventory/items/TowerBlueprint.h"
#include "../../Rpg/inventory/items/TowerBlueprintRare.h"
#include "../../Rpg/inventory/items/TowerBlueprintEpic.h"
#include "../../Rpg/inventory/items/TowerBlueprintMythic.h"
#include <ctime>

Reward RewardSystem::generateReward(int level) {
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
        reward.name = "Tower Blueprint Rare";
        reward.itemId = 3;
        reward.quantity = 1;
    } else {
        reward.name = (roll % 2 == 0) ? "Tower Blueprint Epic" : "Tower Blueprint Mythic";
        reward.itemId = (roll % 2 == 0) ? 4 : 5;
        reward.quantity = 1;
    }

    return reward;
}

