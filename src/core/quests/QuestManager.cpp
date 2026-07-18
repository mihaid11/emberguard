#include "QuestManager.h"
#include <fstream>
#include <memory>
using json = nlohmann::json;

ObjectiveType QuestManager::stringToObjectiveType(const std::string& str) const {
    if (str == "kill_enemy") return ObjectiveType::kill_enemy;
    if (str == "get_item") return ObjectiveType::get_item;
    if (str == "craft_item") return ObjectiveType::craft_item;
    if (str == "talk_to_npc") return ObjectiveType::talk_to_npc;
    if (str == "complete_level_td") return ObjectiveType::complete_level_td;
    if (str == "complete_level") return ObjectiveType::complete_level;

    return ObjectiveType::unknown;
}

bool QuestManager::loadQuestsFromJson(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open())
        return false;

    json j;
    file >> j;

    for (const auto& questJson: j["quests"]) {
        Quest q;
        q.id = questJson.value("id", "");
        q.title = questJson.value("title", "");
        q.description = questJson.value("description", "");
        q.state = QuestState::inactive;

        for (const auto& objJson: questJson["objectives"]) {
            QuestObjective obj;
            obj.type = stringToObjectiveType(objJson.value("type", ""));
            obj.targetId = objJson.value("target_id", "");
            obj.amount = objJson.value("amount", 1);
            obj.currentAmount = 0;
            q.objectives.push_back(obj);
        }

        for (const auto& rewJson: questJson["rewards"]) {
            QuestReward rew;
            rew.type = rewJson.value("type", "");
            rew.targetId = rewJson.value("target_id", "");
            rew.amount = rewJson.value("amount", 1);
            q.rewards.push_back(rew);
        }

        mQuests[q.id] = std::move(q);
    }

    return true;
}

void QuestManager::startQuest(const std::string& questId) {
    auto quest = mQuests.find(questId);
    if (quest != mQuests.end() && quest->second.state == QuestState::inactive)
        quest->second.state = QuestState::active;
}

std::vector<QuestReward> QuestManager::onEvent(const GameEvent& event) {
    std::vector<QuestReward> rewards;

    for (auto& [id, quest]: mQuests) {
        if (quest.state == QuestState::active) {
            if (quest.onEvent(event) && quest.checkCompletion()) {
                quest.state = QuestState::completed;
                rewards.insert(rewards.end(), quest.rewards.begin(), quest.rewards.end());
            }
        }
    }

    return rewards;
}

std::vector<const Quest*> QuestManager::getActiveQuests() const {
    std::vector<const Quest*> activeQuests;
    for (const auto& quest: mQuests)
        if (quest.second.state == QuestState::active)
            activeQuests.push_back(&quest.second);

    return activeQuests;
}
