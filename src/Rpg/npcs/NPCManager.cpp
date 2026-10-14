#include "NPCManager.h"
#include "../gamengine/RPGEngine.h"
#include <iostream>

NPCManager::NPCManager(StoryManager& storyManager, DialogueDatabase& dialogueDatabase, RPGEngine& rpgEngine, Inventory& inventory)
    : mCurrentNPC(nullptr), mShowDialogue(false),
    mStoryManager(storyManager), mDialogueDatabase(dialogueDatabase) {
    mActionExecutor = new DialogueActionExecutor(storyManager, rpgEngine, inventory);
}

NPCManager::~NPCManager() {
    delete mActionExecutor;
}

void NPCManager::addNPC(std::unique_ptr<NPC> npc) {
    mNPCs.push_back(std::move(npc));
}

void NPCManager::refreshNPCDialogues() {
    for (auto& npc : mNPCs) {
        if (mCurrentNPC == npc.get())
            continue;

        npc->refreshDialogue(mDialogueDatabase, mStoryManager);
    }
}

void NPCManager::update(float dt) {
    for (auto& npc : mNPCs)
        npc->update(dt);
}

void NPCManager::render(sf::RenderWindow& window) {
    for (auto& npc : mNPCs)
        npc->render(window);
}

void NPCManager::startDialogueWith(NPC* targetNPC, MainCharacter& player, bool& showDialogue, sf::Text& dialogueText) {
    if (!targetNPC)
        return;

    mCurrentNPC = targetNPC;
    sf::Vector2f playerPosition = player.getPosition();
    int playerAnimation = player.getAnimation();

    // handle NPC facing the player correctly
    if (playerAnimation == 4) {
        if (playerPosition.y <= mCurrentNPC->getPosition().y) {
            mCurrentNPC->setAnimationOpposing(playerAnimation);
        } else {
            player.setAnimation(3);
            mCurrentNPC->setAnimationOpposing(3);
        }
    } else if (playerAnimation == 3) {
        if (playerPosition.y <= mCurrentNPC->getPosition().y) {
            player.setAnimation(4);
            mCurrentNPC->setAnimationOpposing(4);
        } else {
            mCurrentNPC->setAnimationOpposing(playerAnimation);
        }
    } else if (playerAnimation == 2) {
        if (playerPosition.x <= mCurrentNPC->getPosition().x) {
            mCurrentNPC->setAnimationOpposing(playerAnimation);
        } else {
            player.setAnimation(1);
            mCurrentNPC->setAnimationOpposing(1);
        }
    } else if (playerAnimation == 1) {
        if (playerPosition.x <= mCurrentNPC->getPosition().x) {
            player.setAnimation(2);
            mCurrentNPC->setAnimationOpposing(2);
        } else {
            mCurrentNPC->setAnimationOpposing(playerAnimation);
        }
    }

    showDialogue = true;
    mShowDialogue = true;
    dialogueText.setString(mCurrentNPC->getCurrentDialogue());
    stopCurrentNPC();
}

void NPCManager::stopCurrentNPC() {
    if (mCurrentNPC)
        mCurrentNPC->pauseMovement();
}

void NPCManager::resumeCurrentNPC() {
    if (mCurrentNPC) {
        mShowDialogue = false;
        mCurrentNPC->resumeMovement();
        mCurrentNPC->resetDialogue();
        mCurrentNPC = nullptr;
    }
}

void NPCManager::saveNPCStates(std::vector<sf::Vector2f>& npcPositions, std::vector<int>& npcWaypoints) {
    for (auto& npc : mNPCs) {
        npcPositions.push_back(npc->getPosition());
        npcWaypoints.push_back(npc->getCurrentWaypoint());
    }
}

void NPCManager::loadNPCStates(const std::vector<sf::Vector2f>& npcPositions, const std::vector<int>& npcWaypoints) {
    for (size_t i = 0; i < mNPCs.size(); ++i) {
        if (i < npcPositions.size()) {
            mNPCs[i]->setPosition(npcPositions[i]);
            mNPCs[i]->setCurrentWaypoint(npcWaypoints[i]);
        }
    }
}

bool NPCManager::playerClose(const sf::Vector2f& playerPosition) {
    for (auto& npc : mNPCs) {
        if (npc->isPlayerClose(playerPosition))
            return true;
    }
    return false;
}

const std::vector<std::unique_ptr<NPC>>& NPCManager::getNPCs() const {
    return mNPCs;
}

void NPCManager::stopAllNPC() {
    for (auto& npc : mNPCs)
        npc->pauseMovement();
}

void NPCManager::resumeAllNPC() {
    for (auto& npc : mNPCs)
        npc->resumeMovement();
}

NPC* NPCManager::getCurrentNPC() {
    return mCurrentNPC;
}

NPC* NPCManager::getNPCById(const std::string& id) {
    for (auto& npc: mNPCs)
        if (npc->getId() == id)
            return npc.get();
    return nullptr;
}

void NPCManager::clearAllNPCs() {
    mNPCs.clear();
    mCurrentNPC = nullptr;
}

bool NPCManager::currentNPCHasChoices() const {
    if (mCurrentNPC)
        return mCurrentNPC->hasChoices();

    return false;
}

std::vector<DialogueChoice> NPCManager::getCurrentNPCChoices() const {
    if (mCurrentNPC)
        return mCurrentNPC->getChoices();

    return std::vector<DialogueChoice>();
}

void NPCManager::selectChoiceForCurrentNPC(int choiceIndex, bool& showDialogue, sf::Text& dialogueText) {
    if (mCurrentNPC && mCurrentNPC->hasChoices()) {
        auto actions = mCurrentNPC->selectChoiceWithActions(choiceIndex);
        if (mActionExecutor && !actions.empty())
            mActionExecutor->executeActions(actions);

        if (mCurrentNPC->hasMoreDialogue()) {
            dialogueText.setString(mCurrentNPC->getCurrentDialogue());
            stopCurrentNPC();
        } else {
            mShowDialogue = false;
            showDialogue = false;
            mCurrentNPC->resetDialogue();
            mCurrentNPC->resumeMovement();
            mCurrentNPC->refreshDialogue(mDialogueDatabase, mStoryManager);
            mCurrentNPC = nullptr;
        }
    }
}

void NPCManager::interactWithCurrentNPC(bool& showDialogue, sf::Text& dialogueText) {
    if (!mCurrentNPC) return;

    if (mCurrentNPC->hasChoices()) {
        stopCurrentNPC();
        return;
    }

    bool wasLastSegment = mCurrentNPC->getDialogueManager().getCurrentDialogue().isLastSegment();

    mCurrentNPC->advanceDialogue();

    if (mCurrentNPC->hasMoreDialogue()) {
        stopCurrentNPC();
        dialogueText.setString(mCurrentNPC->getCurrentDialogue());
    } else {
        if (wasLastSegment) {
            auto actions = mCurrentNPC->getCurrentDialogueActions();
            if (mActionExecutor && !actions.empty())
                mActionExecutor->executeActions(actions);
        }

        mShowDialogue = false;
        showDialogue = false;
        mCurrentNPC->resetDialogue();
        mCurrentNPC->resumeMovement();
        mCurrentNPC->refreshDialogue(mDialogueDatabase, mStoryManager);
        mCurrentNPC = nullptr;
    }
}
