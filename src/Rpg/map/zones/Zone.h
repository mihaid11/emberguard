#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include "../entities/Entity.h"
#include "../entities/EntityFactory.h"
#include "../../../core/ResourceManager.h"

class Zone {
public:
    Zone(sf::Vector2i coords, const std::string& jsonPath, GameContext& gameContext);

    void update();
    void render(sf::RenderWindow& window);

    const std::vector<std::unique_ptr<Entity>>& getEntities();

    sf::Vector2f getCameraTarget() const;
    bool hasCameraTarget() const;

private:
    void loadFromJson(const std::string& jsonPath, GameContext& gameContext);

    sf::Vector2i mCoords;
    sf::Sprite mBackgroundSprite;
    std::vector<std::unique_ptr<Entity>> mEntities;

    bool mHasCameraTarget;
    sf::Vector2f mCameraTarget;
};

