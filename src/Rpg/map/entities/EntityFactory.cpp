#include "EntityFactory.h"
#include "../buildings/Barrier.h"
#include "../buildings/MCHouse.h"
#include "../buildings/MCHouseInt.h"
#include "../buildings/Bed.h"
#include "../buildings/Chest.h"

std::unique_ptr<Entity> EntityFactory::createEntity(const std::string& type, const sf::Vector2f& pos,
        const sf::Vector2f& size, GameContext& gameContext) {
    if (type == "Barrier") {
        return std::make_unique<Barrier>(pos, size);
    } else if (type == "MCHouse") {
        sf::Vector2f interactSize = sf::Vector2f(43.f, 10.f);
        sf::Vector2f interactPos = pos + sf::Vector2f(size.x / 2.f - interactSize.x / 2.f - 9.f, size.y - interactSize.y / 2.f - 40.f);

        return std::make_unique<MCHouse>(
            pos,
            "assets/sprites/buildings/mcHouseExt.png",
            sf::Vector2f(0.f, 0.f),
            sf::Vector2f(0.f, 0.f),
            1,
            interactPos,
            interactSize,
            gameContext.mainCharacter,
            gameContext.isInsideStructure,
            gameContext.cameraFixedPosition,
            gameContext.changeMap,
            gameContext.structureIndex
        );
    } else if (type == "MCHouseInt") {
        sf::Vector2f interactSize = sf::Vector2f(43.f, 15.f);
        sf::Vector2f interactPos = pos + sf::Vector2f(size.x / 3.f - interactSize.x * 1.7f, size.y - interactSize.y / 2.f - 2.f);

        return std::make_unique<MCHouseInt>(
            pos,
            "assets/sprites/buildings/mcHouseInt.png",
            sf::Vector2f(0.f, 0.f),
            sf::Vector2f(0.f, 0.f),
            1,
            interactPos,
            interactSize,
            gameContext.mainCharacter,
            gameContext.isInsideStructure,
            gameContext.cameraFixedPosition,
            gameContext.changeMap
        );
    } else if (type == "Bed") {
        sf::Vector2f interactSize = sf::Vector2f(72.5f, 25.f);
        sf::Vector2f interactPos = pos + sf::Vector2f(5.f, 5.f);

        return std::make_unique<Bed> (
            pos,
            1,
            "assets/sprites/buildings/bed.png",
            sf::Vector2f(0.f, 0.f),
            sf::Vector2f(0.f, 0.f),
            interactPos,
            interactSize,
            gameContext.timeSystem,
            gameContext.transitionSystem,
            gameContext.gameManager
        );
    } else if (type == "Chest") {
        sf::Vector2f interactSize = sf::Vector2f(50.f, 33.f);
        sf::Vector2f interactPos = pos + sf::Vector2f(5.f, 5.f);

        return std::make_unique<Chest> (
            pos,
            1,
            "assets/sprites/buildings/chest.png",
            sf::Vector2f(0.f, 0.f),
            sf::Vector2f(0.f, 0.f),
            interactPos,
            interactSize,
            gameContext.setChestMenuState
        );
    }

    return nullptr;
}

