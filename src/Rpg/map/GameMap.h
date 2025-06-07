#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "buildings/Entity.h"

class GameMap {
public:
    GameMap();

    template <typename T, typename... Args>
    void addEntity(Args&&... args) {
        mEntities.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    void render(sf::RenderWindow& window);
    bool checkCollision(const sf::FloatRect& objectBounds);
    void update();
    std::vector<std::unique_ptr<Entity>>& getEntities();
    const sf::Vector2f& getSize();
private:
    sf::Vector2f mSize;
    std::vector<std::unique_ptr<Entity>> mEntities;

};

