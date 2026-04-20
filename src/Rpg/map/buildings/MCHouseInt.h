#pragma once
#include <functional>
#include "../entities/Entity.h"
#include "../../mainCharacter/MainCharacter.h"

class MCHouseInt : public Entity {
public:
    MCHouseInt(const sf::Vector2f& position, const std::string filename,
        const sf::Vector2f& collPosition, const sf::Vector2f& collSize, int canInteract,
        const sf::Vector2f& interactPos, const sf::Vector2f& interactSize,
        MainCharacter& mainCharacter, bool& isInsideAStructure, sf::Vector2f& cameraFixedPosition,
        std::function<void(const std::string&)> changeMap);

    void interact() override;
    float getDepthOffset() const override { return -10000.f; }
private:
    MainCharacter& mMainCharacter;
    bool& mIsInsideAStructure;
    sf::Vector2f& mCameraFixedPosition;

    std::function<void(const std::string&)> mChangeMap;
};

