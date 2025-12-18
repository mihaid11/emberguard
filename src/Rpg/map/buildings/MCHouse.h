#pragma once
#include <functional>
#include "../entities/Entity.h"
#include "../../mainCharacter/MainCharacter.h"

class MCHouse : public Entity {
public:
    MCHouse(const sf::Vector2f& position, const std::string filename,
        const sf::Vector2f& collPosition, const sf::Vector2f& collSize, int canInteract,
        const sf::Vector2f& interactPos, const sf::Vector2f& interactSize,
        MainCharacter& mainCharacter, bool& isInsideAStructure, sf::Vector2f& cameraFixedPosition,
        std::function<void(const std::string&)> changeMap, int& structureIndex);

    void interact() override;
private:
    MainCharacter& mMainCharacter;
    bool& mIsInsideAStructure;
    sf::Vector2f& mCameraFixedPosition;

    std::function<void(const std::string&)> mChangeMap;
    int& mStructureIndex;
};

