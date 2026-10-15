#pragma once
#include <functional>
#include "../entities/Entity.h"
#include "../../mainCharacter/MainCharacter.h"

class MCHouse : public Entity {
public:
    MCHouse(const sf::Vector2f& position, const sf::Vector2f& size,
            const std::string filename, const sf::Vector2f& interactPos, const sf::Vector2f& interactSize,
            MainCharacter& mainCharacter, bool& isInsideAStructure, sf::Vector2f& cameraFixedPosition,
            std::function<void(const std::string&)> changeMap, int& structureIndex,
            const sf::Vector2f& collPosition = {0.f, 0.f}, const sf::Vector2f& collSize = {0.f, 0.f});

    void interact() override;
private:
    MainCharacter& mMainCharacter;
    bool& mIsInsideAStructure;
    sf::Vector2f& mCameraFixedPosition;

    std::function<void(const std::string&)> mChangeMap;
    int& mStructureIndex;
};
