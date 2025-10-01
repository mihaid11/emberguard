#include "DialogueActionExecutor.h"
#include "../gamengine/RPGEngine.h"
#include <iostream>

DialogueActionExecutor::DialogueActionExecutor(StoryManager& storyManager, RPGEngine& rpgEngine, Inventory& inventory)
    : mStoryManager(storyManager), mRpgEngine(rpgEngine), mInventory(inventory) {

}

void DialogueActionExecutor::executeActions(const std::vector<DialogueAction>& actions) {
    for (const auto& action : actions)
        executeAction(action);
}

void DialogueActionExecutor::executeAction(const DialogueAction& action) {
    if (action.type == "set_story_flag") {
        mStoryManager.setFlag(action.target, action.value != 0);
    } else if (action.type == "set_menu_flag") {
        mRpgEngine.setFlag(action.target, action.value);
    } else if (action.type == "give_item") {
        // Implement item addition
    } else if (action.type == "remove_item") {
        // Implement item removal
    } else if (action.type == "change_crystals") {
        mRpgEngine.changeCrystals(action.value);
    } else if (action.type == "set_chapter") {
        mStoryManager.setChapter(action.value);
    } else {
        std::cout << "Wrong action type!" << std::endl;
    }
}

