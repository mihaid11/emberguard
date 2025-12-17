#pragma once
#include <memory>
#include <functional>
#include "Entity.h"

class MainCharacter;
class TimeSystem;
class TransitionSystem;
class GameManager;

struct GameContext {
    MainCharacter& mainCharacter;
    bool& isInsideStructure;
    sf::Vector2f& cameraFixedPosition;
    bool& showChestMenu;
    TimeSystem& timeSystem;
    TransitionSystem& transitionSystem;
    GameManager* gameManager;

    std::function<void(const std::string&)> changeMap;
};

class EntityFactory {
public:
    static std::unique_ptr<Entity> createEntity(const std::string& type, const sf::Vector2f& pos,
            const sf::Vector2f& size, GameContext& gameContext);
};
