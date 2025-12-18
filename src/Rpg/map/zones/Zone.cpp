#include "Zone.h"
#include <nlohmann/json.hpp>

Zone::Zone(sf::Vector2i coords, const std::string& jsonPath, GameContext& gameContext)
    : mCoords(coords), mHasCameraTarget(false) {
    loadFromJson(jsonPath, gameContext);
}

void Zone::update() {
    for (auto& entity: mEntities)
        entity->update();
}

const std::vector<std::unique_ptr<Entity>>& Zone::getEntities() {
    return mEntities;
}

void Zone::render(sf::RenderWindow& window) {
    window.draw(mBackgroundSprite);
}

void Zone::loadFromJson(const std::string& jsonPath, GameContext& gameContext) {
    std::ifstream file(jsonPath);
    if (!file.is_open())
        return;

    if (file.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "File exists but is empty: " << jsonPath << std::endl;
        return;
    }

    nlohmann::json j;
    file >> j;

    for (const auto& layer: j["layers"]) {
        std::string type = layer["type"];

        if (type == "imagelayer") {
            std::string backgroundPath = layer["image"];

            if (backgroundPath.find("..") != std::string::npos)
                backgroundPath = "assets" + backgroundPath.substr(2);

            sf::Texture& texture = ResourceManager::getInstance().getTexture(backgroundPath);
            mBackgroundSprite.setTexture(texture);
            mBackgroundSprite.setPosition(mCoords.x * 1024.f, mCoords.y * 1024.f);
        } else if (type == "objectgroup") {
            for (const auto& object: layer["objects"]) {
                std::string entityType = object.value("type", "");

                float x = object["x"];
                float y = object["y"];
                float width = object["width"];
                float height = object["height"];

                if (object.contains("gid"))
                    y -= height;

                sf::Vector2f worldPos(x + (mCoords.x * 1024.f), y + (mCoords.y * 1024.f));
                sf::Vector2f size(width, height);

                if (entityType == "CameraTarget") {
                    mHasCameraTarget = true;
                    mCameraTarget = worldPos;
                    continue;
                }

                auto entity = EntityFactory::createEntity(entityType, worldPos, size, gameContext);
                if (entity)
                    mEntities.push_back(std::move(entity));
            }
        }
    }
}

sf::Vector2f Zone::getCameraTarget() const {
    return mCameraTarget;
}

bool Zone::hasCameraTarget() const {
    return mHasCameraTarget;
}

