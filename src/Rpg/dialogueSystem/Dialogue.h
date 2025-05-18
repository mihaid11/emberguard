#pragma once
#include <SFML/Graphics.hpp>
#include "DialogueSegment.h"

class Dialogue
{
public:
	Dialogue();

	void addSegment(const DialogueSegment& segment);
	std::string getCurrentSegment();
	int getCurrentIndex();
	void indexIncrement();
	bool hasMoreSegments() const;
	void reset();

private:
	std::vector<DialogueSegment> mSegments;
	size_t mCurrentSegmentIndex;
};

