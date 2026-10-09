#include "GarrickStone.h"
#include "../../core/GameManager.h"

GarrickStone::GarrickStone(const sf::Vector2f& position, const std::string& id, GameManager* gameManager)
    : NPC(position, id), mGameManager(gameManager) {
}
