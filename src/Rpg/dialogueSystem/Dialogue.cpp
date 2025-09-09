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
    if (hasMoreSegments())
        ++mCurrentSegmentIndex;
}

bool Dialogue::hasMoreSegments() const {
    return mCurrentSegmentIndex < mSegments.size();
}

void Dialogue::reset() {
    mCurrentSegmentIndex = 0;
}

bool Dialogue::currentSegmentHasChoices() const {
    if (mSegments.empty()) return false;
    return mSegments[mCurrentSegmentIndex].hasChoices();
}

const std::vector<DialogueChoice>& Dialogue::getCurrentChoices() const {
    static std::vector<DialogueChoice> empty;
    if (mSegments.empty()) return empty;
    return mSegments[mCurrentSegmentIndex].getChoices();
}

void Dialogue::applyChoice(int choiceIndex) {
    if (!currentSegmentHasChoices()) return;

    const auto& choices = mSegments[mCurrentSegmentIndex].getChoices();
    if (choiceIndex < 0 || choiceIndex >= choices.size()) return;

    const auto& selectedChoice = choices[choiceIndex];
    addChoiceSegments(selectedChoice);

    if (!mSegments.empty() && mCurrentSegmentIndex < mSegments.size() - 1)
        ++mCurrentSegmentIndex;
}

void Dialogue::addChoiceSegments(const DialogueChoice& choice) {
    mSegments.erase(mSegments.begin() + mCurrentSegmentIndex + 1, mSegments.end());

    for (size_t i = 0; i < choice.nextLines.size(); ++i) {
        DialogueSegment segment(choice.nextLines[i]);

        if (i == choice.nextLines.size() - 1 && !choice.subChoices.empty()) {
            for (const auto& subChoice : choice.subChoices) {
                segment.addChoice(subChoice);
            }
        }

        mSegments.push_back(segment);
    }
}

