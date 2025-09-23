#include "DialogueActionExecutor.h"
#include <iostream>

DialogueActionExecutor::DialogueActionExecutor(StoryManager& storyManager)
    : mStoryManager(storyManager) {

}

void DialogueActionExecutor::executeActions(const std::vector<DialogueAction>& actions) {
    for (const auto& action : actions)
        executeAction(action);
}

void DialogueActionExecutor::executeAction(const DialogueAction& action) {
    if (action.type == "set_story_flag") {
        mStoryManager.setFlag(action.target, action.value != 0);
        std::cout << "Flag set" << std::endl;
    } else if (action.type == "set_menu_flag") {
        std::cout << "Set menu flag: " << action.target << std::endl;
        // Implement menu open/close
    } else if (action.type == "give_item") {
        // Implement item addition
    } else if (action.type == "remove_item") {
        // Implement item removal
    } else if (action.type == "give_crystals") {
        // Implement crystals addition
    } else if (action.type == "set_chapter") {
        mStoryManager.setChapter(action.value);
        std::cout << "Chapter set to " << action.value << std::endl;
    } else {
        std::cout << "Wrong action type!" << std::endl;
    }
}

