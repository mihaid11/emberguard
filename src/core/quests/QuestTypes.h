#pragma once
#include <string>

enum class QuestState {
    inactive,
    active,
    completed
};

enum class ObjectiveType {
    kill_enemy,
    get_item,
    get_tower,
    craft_item,
    analyze_blueprint,
    talk_to_npc,
    deposit_crystals,
    borrow_crystals,
    complete_waves,
    complete_level_td,
    complete_level,
    unknown
};

struct QuestObjective {
    ObjectiveType type;
    std::string targetId;
    std::string description;

    int amount;
    int currentAmount = 0;

    bool isComplete() const { return currentAmount >= amount; }
};

struct QuestReward {
    std::string type;
    std::string targetId;
    int amount;
};

struct GameEvent {
    ObjectiveType type;
    std::string targetId;
    int amount = 1;
};
