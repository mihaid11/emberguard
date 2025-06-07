#include "ElliotMarlowe.h"
#include <iostream>

ElliotMarlowe::ElliotMarlowe(const sf::Vector2f& position)
    : NPC(position), mQuestCompleted(false) {
    mShape.setFillColor(sf::Color::Yellow);

    mGreeting1Dialogue.addSegment(DialogueSegment("Alden, welcome. I trust you've settled in well this morning."));
    mGreeting1Dialogue.addSegment(DialogueSegment("The village thrives because each of us contributes to the whole."));
    mGreeting1Dialogue.addSegment(DialogueSegment("I see potential in you, potential that could help us navigate the challenges ahead."));
    mGreeting1Dialogue.addSegment(DialogueSegment("If you're ready, I have something in mind that could use your talents."));

    mQuest1Dialogue.addSegment(DialogueSegment("Arlon, I want you to do something!"));
    mQuest1Dialogue.addSegment(DialogueSegment("Go to Garrick to take the quest"));

    mQuest1FollowUpDialogue.addSegment(DialogueSegment("Talk to Garrick and then come back!"));

    mReward1Dialogue.addSegment(DialogueSegment("You did great!"));
    mReward1Dialogue.addSegment(DialogueSegment("Here are you rewards!"));

    setDialogue("greeting1", mGreeting1Dialogue);
    setDialogue("quest1", mQuest1Dialogue);
    setDialogue("quest1FollowUp", mQuest1FollowUpDialogue);
    setDialogue("reward1", mReward1Dialogue);

    setActiveDialogue("greeting1");

    mPath.push_back(sf::Vector2f(0.0f, 0.0f));
    mPath.push_back(sf::Vector2f(0.0f, 500.0f));
    mPath.push_back(sf::Vector2f(700.0f, 500.0f));
    mPath.push_back(sf::Vector2f(700.0f, 0.0f));
}

/*
void ElliotMarlowe::interact(GameManager* gameManager) {
    if (gameManager->getQuestManager().isQuestActive("The First Trial")) {
        if (gameManager->getQuestManager().isQuestCompleted("The First Trial")) {
            // Dialogue after completing the quest
            setActiveDialogue("reward1");
            gameManager->getQuestManager().finishQuest("The First Trial");
            gameManager->addCrystals(200); // Reward the player with 200 crystals
        }
        else if (gameManager->getQuestManager().isCurrentStep("The First Trial", 0)) {
            // Initial quest dialogue
            setActiveDialogue("quest1");
        }
        else if (gameManager->getQuestManager().isCurrentStep("The First Trial", 1)) {
            // Dialogue after talking to Vincent
            setActiveDialogue("quest1FollowUp");
        }
    }
    else {
        // Default dialogue when no quest is active
        setActiveDialogue("quest1");
    }
}*/

void ElliotMarlowe::setQuestCompleted(bool completed) {
    mQuestCompleted = completed;
}

bool ElliotMarlowe::isQuestCompleted() const {
    return mQuestCompleted;
}

