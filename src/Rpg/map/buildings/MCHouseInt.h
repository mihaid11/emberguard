#pragma once
#include <functional>
#include "../entities/Entity.h"
#include "../../mainCharacter/MainCharacter.h"

class MCHouseInt : public Entity {
public:
    MCHouseInt(const sf::Vector2f& position, const sf::Vector2f& size,
               const std::string filename, const sf::Vector2f& interactPos, const sf::Vector2f& interactSize,
               MainCharacter& mainCharacter, bool& isInsideAStructure, sf::Vector2f& cameraFixedPosition,
               std::function<void(const std::string&)> changeMap, const sf::Vector2f& collPosition = {0.f, 0.f},
               const sf::Vector2f& collSize = {0.f, 0.f});

    void interact() override;
    float getDepthOffset() const override { return -10000.f; }
private:
    MainCharacter& mMainCharacter;
    bool& mIsInsideAStructure;
    sf::Vector2f& mCameraFixedPosition;

    std::function<void(const std::string&)> mChangeMap;
};
