#pragma once
#include <string>
#include <vector>
#include "QuestTypes.h"

class Quest {
public:
    std::string id;
    std::string title;
    std::string description;
    QuestState state = QuestState::inactive;

    std::vector<QuestObjective> objectives;
    std::vector<QuestReward> rewards;

    bool onEvent(const GameEvent& event);
    bool checkCompletion() const;
};
