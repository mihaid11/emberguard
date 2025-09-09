#pragma once
#include <SFML/Graphics.hpp>
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

private:
    std::string mText;
    std::vector<DialogueChoice> mChoices;
};

