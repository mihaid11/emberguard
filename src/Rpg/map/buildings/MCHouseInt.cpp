#include "MCHouseInt.h"
#include <iostream>

MCHouseInt::MCHouseInt(const sf::Vector2f& position, const std::string filename, const sf::Vector2f& collPosition,
                       const sf::Vector2f& collSize, int canInteract, const sf::Vector2f& interactPos,
                       const sf::Vector2f& interactSize, MainCharacter& mainCharacter, bool& isInsideAStructure,
                       sf::Vector2f& cameraFixedPosition)
    : Entity(position, filename, collPosition, collSize, canInteract, interactPos, interactSize), mMainCharacter(mainCharacter),
    mIsInsideAStructure(isInsideAStructure), mCameraFixedPosition(cameraFixedPosition) {
    mSprite.setScale(sf::Vector2f(1.65f, 1.65f));
}

void MCHouseInt::interact() {
    std::cout << "Teleporting MC from house!" << std::endl;
    mMainCharacter.setPosition(sf::Vector2f(343.f, 553.f));
    mMainCharacter.setAnimation(4);
    mIsInsideAStructure = false;
    mCameraFixedPosition = { 0.f, 0.f };
}

