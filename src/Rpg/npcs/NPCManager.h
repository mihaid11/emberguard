#pragma once
#include <memory>
#include <vector>
#include "NPC.h"
#include "../dialogueSystem/DialogueDatabase.h"
#include "../story/StoryManager.h"
#include "../dialogueSystem/DialogueActionExecutor.h"
#include "../mainCharacter/MainCharacter.h"
#include "../inventory/Inventory.h"

class RPGEngine;

class NPCManager {
public:
    NPCManager(StoryManager& storyManager, DialogueDatabase& dialogueDatabase, RPGEngine& rpgEngine, Inventory& inventory);
    ~NPCManager();

    void addNPC(std::unique_ptr<NPC> npc);
    void refreshNPCDialogues();
    void update(float dt);
    void render(sf::RenderWindow& window);
    void handleInteraction(MainCharacter& player, bool& mShowDialogue, sf::Text& dialogueText);
    void interactWithCurrentNPC(bool& mShowDialogue, sf::Text& dialogueText);
    void resumeCurrentNPC();
    void stopCurrentNPC();
    void saveNPCStates(std::vector<sf::Vector2f>& npcPositions, std::vector<int>& npcWaypoints);
    void loadNPCStates(const std::vector<sf::Vector2f>& npcPositions, const std::vector<int>& npcWaypoints);
    bool playerClose(const sf::Vector2f& playerPosition);
    const std::vector<std::unique_ptr<NPC>>& getNPCs() const;

    void stopAllNPC();
    void resumeAllNPC();

    NPC* getCurrentNPC();

    bool currentNPCHasChoices() const;
    std::vector<DialogueChoice> getCurrentNPCChoices() const;
    void selectChoiceForCurrentNPC(int choiceIndex, bool& showDialogue, sf::Text& dialogueText);

private:
    std::vector<std::unique_ptr<NPC>> mNPCs;
    NPC* mCurrentNPC;
    StoryManager& mStoryManager;
    DialogueDatabase& mDialogueDatabase;
    DialogueActionExecutor* mActionExecutor;
    bool mShowDialogue;
};

