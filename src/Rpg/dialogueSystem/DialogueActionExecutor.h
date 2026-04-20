#pragma once
#include "../story/StoryManager.h"
#include "../inventory/Inventory.h"
#include "DialogueAction.h"
#include <vector>

class RPGEngine;

class DialogueActionExecutor {
public:
    DialogueActionExecutor(StoryManager& storyManager, RPGEngine& rpgEngine, Inventory& inventory);

    void executeActions(const std::vector<DialogueAction>& actions);
    void executeAction(const DialogueAction& action);

private:
    StoryManager& mStoryManager;
    RPGEngine& mRpgEngine;
    Inventory& mInventory;
};
