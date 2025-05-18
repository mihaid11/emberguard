#include "DialogueSegment.h"

DialogueSegment::DialogueSegment(const std::string& text)
	: mText(text) {
}

std::string DialogueSegment::getText() const
{
	return mText;
}