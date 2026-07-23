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

    void getQuestSaveData(std::vector<std::string>& ids, std::vector<int>& states, std::vector<std::vector<int>>& objProgress) const;
    void loadQuestSaveData(const std::vector<std::string>& ids, const std::vector<int>& states, const std::vector<std::vector<int>>& objProgress);
    void resetQuests();

private:
    std::unordered_map<std::string, Quest> mQuests;

    ObjectiveType stringToObjectiveType(const std::string& str) const;
};
