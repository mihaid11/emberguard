#pragma once
#include "DialogueAction.h"
#include <string>
#include <vector>

struct DialogueChoice {
    std::string text;
    std::vector<std::string> nextLines;
    std::vector<DialogueChoice> subChoices;
    std::vector<DialogueAction> actions;
};

