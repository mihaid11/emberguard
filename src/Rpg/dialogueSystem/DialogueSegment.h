#pragma once
#include <SFML/Graphics.hpp>

class DialogueSegment
{
public:
    DialogueSegment(const std::string& text);

    std::string getText() const;

private:
    std::string mText;
};

