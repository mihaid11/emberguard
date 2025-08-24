#pragma once
#include "NPC.h"

class GameManager;

class GarrickStone : public NPC {
public:
    GarrickStone(const sf::Vector2f& position, const std::string& id, GameManager* gameManager);

private:
    GameManager* mGameManager;
};

