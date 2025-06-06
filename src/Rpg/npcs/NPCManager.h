#pragma once
#include <memory>
#include <vector>
#include "NPC.h"
#include "../mainCharacter/MainCharacter.h"

class NPCManager
{
public:
    NPCManager();

    void addNPC(std::unique_ptr<NPC> npc);
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

private:
    std::vector<std::unique_ptr<NPC>> mNPCs;
    NPC* mCurrentNPC;
    bool mShowDialogue;
};

