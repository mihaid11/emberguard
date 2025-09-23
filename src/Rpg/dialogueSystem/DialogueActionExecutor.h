#pragma once
#include "../story/StoryManager.h"
#include "DialogueAction.h"
#include <vector>

class DialogueActionExecutor {
public:
    DialogueActionExecutor(StoryManager& storyManager);

    void executeActions(const std::vector<DialogueAction>& actions);
    void executeAction(const DialogueAction& action);

private:
    StoryManager& mStoryManager;
};

