#include "DialogueSegment.h"

DialogueSegment::DialogueSegment(const std::string& text)
    : mText(text) {
}

std::string DialogueSegment::getText() const {
    return mText;
}

void DialogueSegment::addChoice(const DialogueChoice& choice) {
    mChoices.push_back(choice);
}

const std::vector<DialogueChoice>& DialogueSegment::getChoices() const {
    return mChoices;
}

bool DialogueSegment::hasChoices() const {
    return !mChoices.empty();
}

void DialogueSegment::addAction(const DialogueAction& action) {
    mActions.push_back(action);
}

const std::vector<DialogueAction>& DialogueSegment::getActions() const {
    return mActions;
}

bool DialogueSegment::hasActions() const {
    return !mActions.empty();
}

