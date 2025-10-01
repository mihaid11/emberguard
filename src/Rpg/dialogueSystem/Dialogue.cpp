#include "Dialogue.h"

Dialogue::Dialogue() : mCurrentSegmentIndex(0) {
}

void Dialogue::addSegment(const DialogueSegment& segment) {
    mSegments.push_back(segment);
}

std::string Dialogue::getCurrentSegment() {
    if (mSegments.empty() || mCurrentSegmentIndex >= mSegments.size())
        return "";
    return mSegments[mCurrentSegmentIndex].getText();
}

int Dialogue::getCurrentIndex() {
    return (int)mCurrentSegmentIndex;
}

void Dialogue::indexIncrement() {
    if (mCurrentSegmentIndex < mSegments.size())
        ++mCurrentSegmentIndex;
}

bool Dialogue::hasMoreSegments() const {
    return !mSegments.empty() && mCurrentSegmentIndex < mSegments.size();
}

void Dialogue::reset() {
    mCurrentSegmentIndex = 0;
}

bool Dialogue::currentSegmentHasChoices() const {
    if (mSegments.empty() || mCurrentSegmentIndex >= mSegments.size()) return false;
    return mSegments[mCurrentSegmentIndex].hasChoices();
}

const std::vector<DialogueChoice>& Dialogue::getCurrentChoices() const {
    static std::vector<DialogueChoice> empty;
    if (mSegments.empty() || mCurrentSegmentIndex >= mSegments.size()) return empty;
    return mSegments[mCurrentSegmentIndex].getChoices();
}

void Dialogue::applyChoice(int choiceIndex) {
    if (!currentSegmentHasChoices()) return;

    const auto& choices = mSegments[mCurrentSegmentIndex].getChoices();
    if (choiceIndex < 0 || choiceIndex >= choices.size()) return;

    const auto& selectedChoice = choices[choiceIndex];
    addChoiceSegments(selectedChoice);

    if (!mSegments.empty())
        ++mCurrentSegmentIndex;
}

std::vector<DialogueAction> Dialogue::applyChoiceWithActions(int choiceIndex) {
    if (!currentSegmentHasChoices()) return std::vector<DialogueAction>();

    const auto& choices = mSegments[mCurrentSegmentIndex].getChoices();
    if (choiceIndex < 0 || choiceIndex >= choices.size())
        return std::vector<DialogueAction>();

    const auto& selectedChoice = choices[choiceIndex];
    std::vector<DialogueAction> actions = selectedChoice.actions;
    addChoiceSegments(selectedChoice);

    if (!mSegments.empty())
        ++mCurrentSegmentIndex;

    return actions;
}

void Dialogue::addChoiceSegments(const DialogueChoice& choice) {
    if (mCurrentSegmentIndex < mSegments.size())
        mSegments.erase(mSegments.begin() + mCurrentSegmentIndex + 1, mSegments.end());

    for (size_t i = 0; i < choice.nextLines.size(); ++i) {
        DialogueSegment segment(choice.nextLines[i]);

        if (i == choice.nextLines.size() - 1 && !choice.subChoices.empty())
            for (const auto& subChoice : choice.subChoices)
                segment.addChoice(subChoice);

        mSegments.push_back(segment);
    }
}

std::vector<DialogueAction> Dialogue::getCurrentSegmentActions() const {
    if (mSegments.empty())
        return std::vector<DialogueAction>();

    if (mCurrentSegmentIndex >= mSegments.size() && !mSegments.empty())
        return mSegments.back().getActions();

    if (mCurrentSegmentIndex < mSegments.size())
        return mSegments[mCurrentSegmentIndex].getActions();

    return std::vector<DialogueAction>();
}

std::vector<DialogueAction> Dialogue::getChoiceActions(int choiceIndex) const {
    if (!currentSegmentHasChoices()) return std::vector<DialogueAction>();

    const auto& choices = mSegments[mCurrentSegmentIndex].getChoices();
    if (choiceIndex < 0 || choiceIndex >= choices.size())
        return std::vector<DialogueAction>();

    return choices[choiceIndex].actions;
}

int Dialogue::getSegmentCount() const {
    return (int)mSegments.size();
}

bool Dialogue::isLastSegment() const {
    if (mSegments.empty()) return true;
    return mCurrentSegmentIndex == mSegments.size() - 1;
}

