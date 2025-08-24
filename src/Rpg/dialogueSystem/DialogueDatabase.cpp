#include "DialogueDatabase.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool DialogueDatabase::loadDialogueFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open dialogue file: " << filepath << "\n";
        return false;
    }

    json j;
    file >> j;

    NPCDialogue npcDialogue;
    npcDialogue.npcId = j.value("npc_id", "");

    for (const auto& dialogue : j["dialogues"]) {
        DialogueEntry entry;
        entry.conditions = dialogue.value("conditions", json::object());

        for (const auto& line : dialogue["lines"]) {
            entry.lines.push_back(line.get<std::string>());
        }

        npcDialogue.dialogues.push_back(std::move(entry));
    }

    if (npcDialogue.npcId.empty()) {
        std::cerr << "Dialogue file missing npc_id: " << filepath << "\n";
        return false;
    }

    mDialogueData[npcDialogue.npcId] = std::move(npcDialogue);
    return true;
}

bool DialogueDatabase::conditionsMatch(const json& conditions, const StoryManager& storyManager) const {
    for (auto it = conditions.begin(); it != conditions.end(); ++it) {
        const std::string key = it.key();

        if (key == "chapter") {
            int chapterRequired = it.value().get<int>();
            if (storyManager.getChapter() != chapterRequired) return false;
        } else {
            bool flagRequired = it.value().get<bool>();
            if (storyManager.getFlag(key) != flagRequired) return false;
        }
    }
    return true;
}

std::optional<Dialogue> DialogueDatabase::getDialogueForNPC(const std::string& npcId, const StoryManager& storyManager) const {
    auto it = mDialogueData.find(npcId);
    if (it == mDialogueData.end()) {
        return std::nullopt;
    }

    const NPCDialogue& npcDialogue = it->second;

    // Notice that it chooses the first dialogue in the json file that matches the conditions, so more specific dialogues should be put at the top of the file to be chosen
    for (const DialogueEntry& entry : npcDialogue.dialogues) {
        if (conditionsMatch(entry.conditions, storyManager)) {
            Dialogue dialogue;
            for (const std::string& line : entry.lines) {
                dialogue.addSegment(DialogueSegment(line));
            }
            return dialogue;
        }
    }

    return std::nullopt;
}
