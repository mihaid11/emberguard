#include "GarrickStone.h"
#include "../../GameManager.h"

GarrickStone::GarrickStone(const sf::Vector2f& position, GameManager* gameManager)
    : NPC(position), mGameManager(gameManager)
{
    mShape.setFillColor(sf::Color(180, 0, 60, 255));

    mGreeting1Dialogue.addSegment(DialogueSegment("Alden. You're here early. Good. The village needs more people who aren't afraid to get their hands dirty."));
    mGreeting1Dialogue.addSegment(DialogueSegment("I've got a task for you , if you're up for it"));
    mGreeting1Dialogue.addSegment(DialogueSegment("Nothing too dangerous... yet. But it'll help you learn the ropes. The village doesn't defend itself, after all."));

    setDialogue("greeting1", mGreeting1Dialogue);

    setActiveDialogue("greeting1");

    mPath.push_back(sf::Vector2f(-100.0f, -50.0f));
    mPath.push_back(sf::Vector2f(300.0f, -50.0f));
    mPath.push_back(sf::Vector2f(300.0f, 800.0f));
    mPath.push_back(sf::Vector2f(100.0f, 100.0f));
}
/*
void GarrickStone::interact(GameManager* gameManager) {
    if (gameManager->getQuestManager().isQuestActive("The First Trial") &&
        gameManager->getQuestManager().isCurrentStep("The First Trial", 1)) {
        // Dialogue for Vincent during the quest
        setDialogue("task1", mTask1Dialogue);
    }
    else {
        // Default dialogue
        setDialogue("greeting1", mGreeting1Dialogue);
    }
}*/
