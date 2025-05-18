#include "DialogueManager.h"
#include <iostream>

DialogueManager::DialogueManager()
{
}

void DialogueManager::startDialogue(const Dialogue& dialogue)
{
    mCurrentDialogue = dialogue;
}

std::string DialogueManager::getCurrentDialogueText()
{
    return mCurrentDialogue.getCurrentSegment();
}

Dialogue& DialogueManager::getCurrentDialogue()
{
    return mCurrentDialogue;
}

bool DialogueManager::isDialogueInProgress() const
{
    return mCurrentDialogue.hasMoreSegments();
}

void DialogueManager::resetDialogue()
{
    mCurrentDialogue.reset();
}

bool DialogueManager::isDialogueCompleted(const std::string& npcName, const std::string& questName)
{
    return false;
}

void DialogueManager::updateNPCDialogue(const std::string& npcName, const std::string& dialogueKey)
{
}

void DialogueManager::setNPCDialogue(const std::string& npcName, const std::string& dialogueKey)
{
}
