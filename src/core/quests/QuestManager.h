#pragma once
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>
#include "Quest.h"

class QuestManager {
public:
    QuestManager() = default;

    bool loadQuestsFromJson(const std::string& filePath);

    void startQuest(const std::string& questId);
    std::vector<QuestReward> onEvent(const GameEvent& event);

    std::vector<const Quest*> getActiveQuests() const;

private:
    std::unordered_map<std::string, Quest> mQuests;

    ObjectiveType stringToObjectiveType(const std::string& str) const;
};
