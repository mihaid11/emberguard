#include "NPCManager.h"
#include <iostream>

NPCManager::NPCManager() : mCurrentNPC(nullptr), mShowDialogue(false) {

}

void NPCManager::addNPC(std::unique_ptr<NPC> npc) {
    mNPCs.push_back(std::move(npc));
}

void NPCManager::update(float dt) {
    for (auto& npc : mNPCs) {
        npc->update(dt);
    }
}

void NPCManager::render(sf::RenderWindow& window) {
    for (auto& npc : mNPCs) {
        npc->render(window);
    }
}

void NPCManager::handleInteraction(MainCharacter& player, bool& showDialogue,
    sf::Text& dialogueText) {
    sf::Vector2f playerPosition = player.getPosition();
    int playerAnimation = player.getAnimation();
    for (auto& npc : mNPCs) {
        if (npc->isPlayerClose(playerPosition)) {
            mCurrentNPC = npc.get();

            // handle NPC facing the player correctly
            if (playerAnimation == 4) {
                if (playerPosition.y <= mCurrentNPC->getPosition().y)
                    npc->setAnimationOpposing(playerAnimation);
                else {
                    player.setAnimation(3);
                    npc->setAnimationOpposing(3);
                }
            }
            else if (playerAnimation == 3) {
                if (playerPosition.y <= mCurrentNPC->getPosition().y) {
                    player.setAnimation(4);
                    npc->setAnimationOpposing(4);
                }
                else
                    npc->setAnimationOpposing(playerAnimation);
            }
            else if (playerAnimation == 2) {
                if (playerPosition.x <= mCurrentNPC->getPosition().x)
                    npc->setAnimationOpposing(playerAnimation);
                else {
                    player.setAnimation(1);
                    npc->setAnimationOpposing(1);
                }
            }
            else if (playerAnimation == 1) {
                if (playerPosition.x <= mCurrentNPC->getPosition().x) {
                    player.setAnimation(2);
                    npc->setAnimationOpposing(2);
                }
                else
                    npc->setAnimationOpposing(playerAnimation);
            }

            showDialogue = true;
            mShowDialogue = true;
            dialogueText.setString(mCurrentNPC->getCurrentDialogue());
            //std::cout << "Interacting with NPC: " << typeid(*mCurrentNPC).name() << std::endl;
            //std::cout << "Current Dialogue: " << mCurrentNPC->getCurrentDialogue() << std::endl;

            break;
        }
    }
}

void NPCManager::interactWithCurrentNPC(bool& showDialogue, sf::Text& dialogueText) {
    if (mCurrentNPC) {
        if (mCurrentNPC->hasMoreDialogue()) {
            stopCurrentNPC();
            dialogueText.setString(mCurrentNPC->getCurrentDialogue());
            std::cout << "Current Dialogue: " << mCurrentNPC->getCurrentDialogue() << std::endl;
            mCurrentNPC->advanceDialogue();
        }
        else {
            mShowDialogue = false;
            showDialogue = false;
            mCurrentNPC->resetDialogue();
            mCurrentNPC->resumeMovement();
            mCurrentNPC = nullptr;
        }
    }
}

void NPCManager::stopCurrentNPC() {
    if (mCurrentNPC) {
        mCurrentNPC->pauseMovement();
    }
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

bool NPCManager::playerClose(const sf::Vector2f& playerPosition)
{
    for (auto& npc : mNPCs) {
        if (npc->isPlayerClose(playerPosition))
            return true;
    }
    return false;
}

const std::vector<std::unique_ptr<NPC>>& NPCManager::getNPCs() const
{
    return mNPCs;
}

void NPCManager::stopAllNPC()
{
    for (auto& npc : mNPCs) {
        npc->pauseMovement();
    }
}

void NPCManager::resumeAllNPC()
{
    for (auto& npc : mNPCs) {
        npc->resumeMovement();
    }
}

NPC* NPCManager::getCurrentNPC()
{
    return mCurrentNPC;
}
