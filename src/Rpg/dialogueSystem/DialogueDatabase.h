#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>
#include "Dialogue.h"
#include "../story/StoryManager.h"

class DialogueDatabase {
public:
    DialogueDatabase() = default;

    bool loadDialogueFromFile(const std::string& filepath);

    std::optional<Dialogue> getDialogueForNPC(
        const std::string& npcId,
        const StoryManager& storyManager
    ) const;

private:
    struct DialogueEntry {
        nlohmann::json conditions;
        std::vector<std::string> lines;
    };

    struct NPCDialogue {
        std::string npcId;
        std::vector<DialogueEntry> dialogues;
    };

    std::unordered_map<std::string, NPCDialogue> mDialogueData;

    bool conditionsMatch(const nlohmann::json& conditions, const StoryManager& storyManager) const;
};
