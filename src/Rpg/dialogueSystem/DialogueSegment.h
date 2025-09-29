#pragma once
#include <SFML/Graphics.hpp>
#include "DialogueAction.h"
#include "DialogueChoice.h"
#include <vector>
#include <string>

class DialogueSegment {
public:
    DialogueSegment(const std::string& text);

    std::string getText() const;

    void addChoice(const DialogueChoice& choice);
    const std::vector<DialogueChoice>& getChoices() const;
    bool hasChoices() const;

    void addAction(const DialogueAction& action);
    const std::vector<DialogueAction>& getActions() const;
    bool hasActions() const;

private:
    std::string mText;
    std::vector<DialogueChoice> mChoices;
    std::vector<DialogueAction> mActions;
};

