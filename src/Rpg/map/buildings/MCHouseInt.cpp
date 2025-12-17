#include "MCHouseInt.h"
#include <iostream>

MCHouseInt::MCHouseInt(const sf::Vector2f& position, const std::string filename, const sf::Vector2f& collPosition,
                       const sf::Vector2f& collSize, int canInteract, const sf::Vector2f& interactPos,
                       const sf::Vector2f& interactSize, MainCharacter& mainCharacter, bool& isInsideAStructure,
                       sf::Vector2f& cameraFixedPosition, std::function<void(const std::string&)> changeMap)
    : Entity(position, filename, collPosition, collSize, canInteract, interactPos, interactSize), mMainCharacter(mainCharacter),
    mIsInsideAStructure(isInsideAStructure), mCameraFixedPosition(cameraFixedPosition), mChangeMap(changeMap) {
}

void MCHouseInt::interact() {
    mMainCharacter.setPosition(sf::Vector2f(295.f, 262.f));
    mMainCharacter.setAnimation(4);
    mIsInsideAStructure = false;
    mCameraFixedPosition = { 0.f, 0.f };

    mChangeMap("open_world");
}

