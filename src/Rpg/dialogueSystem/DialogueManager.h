#pragma once
#include <SFML/Graphics.hpp>
#include "Dialogue.h"
#include <vector>
#include <string>

class DialogueManager {
public:
    DialogueManager();

    void startDialogue(const Dialogue& dialogue);
    std::string getCurrentDialogueText();
    Dialogue& getCurrentDialogue();
    bool isDialogueInProgress() const;
    void resetDialogue();

    bool currentHasChoices() const;
    std::vector<DialogueChoice> getChoices() const;
    void choose(int index);

private:
    Dialogue mCurrentDialogue;
};

