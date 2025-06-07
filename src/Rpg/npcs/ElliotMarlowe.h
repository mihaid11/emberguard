#pragma once
#include "NPC.h"

class ElliotMarlowe : public NPC {
public:
    ElliotMarlowe(const sf::Vector2f& position);

    //void interact(GameManager* gameManager);

    void setQuestCompleted(bool completed);
    bool isQuestCompleted() const;

private:
    Dialogue mGreeting1Dialogue;
    Dialogue mQuest1Dialogue;
    Dialogue mQuest1FollowUpDialogue;
    Dialogue mReward1Dialogue;

    bool mQuestCompleted;
};

