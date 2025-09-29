#include "DialogueManager.h"
#include <iostream>

DialogueManager::DialogueManager() {
}

void DialogueManager::startDialogue(const Dialogue& dialogue) {
    mCurrentDialogue = dialogue;
}

std::string DialogueManager::getCurrentDialogueText() {
    return mCurrentDialogue.getCurrentSegment();
}

Dialogue& DialogueManager::getCurrentDialogue() {
    return mCurrentDialogue;
}

bool DialogueManager::isDialogueInProgress() const {
    return mCurrentDialogue.hasMoreSegments();
}

void DialogueManager::resetDialogue() {
    mCurrentDialogue.reset();
}

bool DialogueManager::currentHasChoices() const {
    return mCurrentDialogue.currentSegmentHasChoices();
}

std::vector<DialogueChoice> DialogueManager::getChoices() const {
    return mCurrentDialogue.getCurrentChoices();
}

void DialogueManager::choose(int index) {
    mCurrentDialogue.applyChoice(index);
}

std::vector<DialogueAction> DialogueManager::chooseWithActions(int index) {
    return mCurrentDialogue.applyChoiceWithActions(index);
}

std::vector<DialogueAction> DialogueManager::getCurrentActions() const {
    return mCurrentDialogue.getCurrentSegmentActions();
}

