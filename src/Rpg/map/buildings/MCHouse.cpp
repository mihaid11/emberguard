#include "MCHouse.h"
#include <iostream>

MCHouse::MCHouse(const sf::Vector2f& position, const std::string filename, const sf::Vector2f& collPosition,
                 const sf::Vector2f& collSize, int canInteract, const sf::Vector2f& interactPos,
                 const sf::Vector2f& interactSize, MainCharacter& mainCharacter, bool& isInsideAStructure,
                 sf::Vector2f& cameraFixedPosition)
    : Entity(position, filename, collPosition, collSize, canInteract, interactPos, interactSize), mMainCharacter(mainCharacter),
    mIsInsideAStructure(isInsideAStructure), mCameraFixedPosition(cameraFixedPosition) {

}

void MCHouse::interact() {
    std::cout << "Teleporting MC to house!" << std::endl;
    mMainCharacter.setPosition(sf::Vector2f(-900.f, -600.f));
    mMainCharacter.setAnimation(3);
    mIsInsideAStructure = true;
    mCameraFixedPosition = { -760.f, -760.f };
}

