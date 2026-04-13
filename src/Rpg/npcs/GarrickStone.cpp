#include "GarrickStone.h"
#include "../../core/GameManager.h"

GarrickStone::GarrickStone(const sf::Vector2f& position, const std::string& id, GameManager* gameManager)
    : NPC(position, id), mGameManager(gameManager) {
    mPath.push_back(sf::Vector2f(-100.0f, -50.0f));
    mPath.push_back(sf::Vector2f(300.0f, -50.0f));
    mPath.push_back(sf::Vector2f(300.0f, 800.0f));
    mPath.push_back(sf::Vector2f(100.0f, 100.0f));
}

