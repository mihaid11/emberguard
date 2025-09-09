#pragma once
#include <SFML/Graphics.hpp>
#include "DialogueSegment.h"

class Dialogue {
public:
    Dialogue();

    void addSegment(const DialogueSegment& segment);
    std::string getCurrentSegment();
    int getCurrentIndex();
    void indexIncrement();
    bool hasMoreSegments() const;
    void reset();

    bool currentSegmentHasChoices() const;
    const std::vector<DialogueChoice>& getCurrentChoices() const;
    void applyChoice(int choiceIndex);

private:
    std::vector<DialogueSegment> mSegments;
    size_t mCurrentSegmentIndex;

    void addChoiceSegments(const DialogueChoice& choice);
};

