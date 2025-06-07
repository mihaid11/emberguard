#pragma once
#include "NPC.h"

class GameManager;

class GarrickStone : public NPC {
public:
    GarrickStone(const sf::Vector2f& position, GameManager* gameManager);

    //void interact(GameManager* gameManager);

private:
    Dialogue mGreeting1Dialogue;

    GameManager* mGameManager;
};

