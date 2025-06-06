#include "GameMap.h"

GameMap::GameMap() : mSize(sf::Vector2f(10000.0f, 10000.0f))
{

}

void GameMap::render(sf::RenderWindow& window)
{
    for (auto& entity : mEntities) {
        entity->render(window);
    }
}

bool GameMap::checkCollision(const sf::FloatRect& objectBounds) {
    for (const auto& entity : mEntities) {
        if (entity->getBounds().intersects(objectBounds)) {
            return true;  // Collision detected
        }
    }
    return false;
}

void GameMap::update()
{
    for (auto& entity : mEntities)
        entity->update();
}

std::vector<std::unique_ptr<Entity>>& GameMap::getEntities()
{
    return mEntities;
}

const sf::Vector2f& GameMap::getSize()
{
    return mSize;
}
