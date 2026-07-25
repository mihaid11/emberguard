#include "QuestManager.h"
#include "Quest.h"
#include <fstream>
#include <memory>
using json = nlohmann::json;

ObjectiveType QuestManager::stringToObjectiveType(const std::string& str) const {
    if (str == "kill_enemy") return ObjectiveType::kill_enemy;
    if (str == "get_item") return ObjectiveType::get_item;
    if (str == "get_tower") return ObjectiveType::get_tower;
    if (str == "craft_item") return ObjectiveType::craft_item;
    if (str == "analyze_blueprint") return ObjectiveType::analyze_blueprint;
    if (str == "deposit_crystals") return ObjectiveType::deposit_crystals;
    if (str == "borrow_crystals") return ObjectiveType::borrow_crystals;
    if (str == "talk_to_npc") return ObjectiveType::talk_to_npc;
    if (str == "complete_waves") return ObjectiveType::complete_waves;
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
            obj.description = objJson.value("description", "");

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

std::vector<QuestReward> QuestManager::onEvent(const GameEvent& event, std::vector<std::string>& completedQuests) {
    std::vector<QuestReward> rewards;

    for (auto& [id, quest]: mQuests) {
        if (quest.state == QuestState::active) {
            if (quest.onEvent(event) && quest.checkCompletion()) {
                quest.state = QuestState::completed;
                completedQuests.push_back(quest.title);
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

std::string QuestManager::getQuestTitle(const std::string& questId) const {
    auto it = mQuests.find(questId);

    if (it != mQuests.end())
        return it->second.title;
    return "";
}

void QuestManager::getQuestSaveData(std::vector<std::string>& ids, std::vector<int>& states, std::vector<std::vector<int>>& objProgress) const {
    for (const auto& [id, quest]: mQuests) {
        if (quest.state != QuestState::inactive) {
            ids.push_back(id);
            states.push_back(static_cast<int>(quest.state));

            std::vector<int> prog;
            for (const auto& obj: quest.objectives)
                prog.push_back(obj.currentAmount);
            objProgress.push_back(prog);
        }
    }
}

void QuestManager::loadQuestSaveData(const std::vector<std::string>& ids, const std::vector<int>& states, const std::vector<std::vector<int>>& objProgress) {
    resetQuests();

    for (int i = 0; i < ids.size(); ++i) {
        auto it = mQuests.find(ids[i]);

        if (it != mQuests.end()) {
            it->second.state = static_cast<QuestState>(states[i]);
            for (int j = 0; j < objProgress[i].size() && j < it->second.objectives.size(); ++j)
                it->second.objectives[j].currentAmount = objProgress[i][j];
        }
    }
}

void QuestManager::resetQuests() {
    for (auto& [id, quest]: mQuests) {
        quest.state = QuestState::inactive;
        for (auto& obj: quest.objectives)
            obj.currentAmount = 0;
    }
}
