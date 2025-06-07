#include "Dialogue.h"

Dialogue::Dialogue() : mCurrentSegmentIndex(0) {
}

void Dialogue::addSegment(const DialogueSegment& segment) {
    mSegments.push_back(segment);
}

std::string Dialogue::getCurrentSegment() {
    return mSegments[mCurrentSegmentIndex].getText();
}

int Dialogue::getCurrentIndex() {
    return (int)mCurrentSegmentIndex;
}

void Dialogue::indexIncrement() {
    ++mCurrentSegmentIndex;
}

bool Dialogue::hasMoreSegments() const {
    return mCurrentSegmentIndex < mSegments.size();
}

void Dialogue::reset() {
    mCurrentSegmentIndex = 0;
}

