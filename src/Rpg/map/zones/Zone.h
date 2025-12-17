#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include "../entities/Entity.h"
#include "../entities/EntityFactory.h"

class Zone {
public:
    Zone(sf::Vector2i coords, const std::string& jsonPath, GameContext& gameContext);

    void update();
    void render(sf::RenderWindow& window);

    const std::vector<std::unique_ptr<Entity>>& getEntities();

private:
    void loadFromJson(const std::string& jsonPath, GameContext& gameContext);

    sf::Vector2i mCoords;
    sf::Texture mBackgroundTexture;
    sf::Sprite mBackgroundSprite;
    std::vector<std::unique_ptr<Entity>> mEntities;
};

