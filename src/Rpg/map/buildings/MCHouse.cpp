#include "MCHouse.h"
#include <iostream>

MCHouse::MCHouse(const sf::Vector2f& position, const std::string filename, const sf::Vector2f& collPosition,
                 const sf::Vector2f& collSize, int canInteract, const sf::Vector2f& interactPos,
                 const sf::Vector2f& interactSize, MainCharacter& mainCharacter, bool& isInsideAStructure,
                 sf::Vector2f& cameraFixedPosition, std::function<void(const std::string&)> changeMap)
    : Entity(position, filename, collPosition, collSize, canInteract, interactPos, interactSize), mMainCharacter(mainCharacter),
    mIsInsideAStructure(isInsideAStructure), mCameraFixedPosition(cameraFixedPosition), mChangeMap(changeMap) {

}

void MCHouse::interact() {
    mMainCharacter.setPosition(sf::Vector2f(77.f - 1024.f, 290.f - 1024.f));
    mMainCharacter.setAnimation(3);
    mIsInsideAStructure = true;
    mCameraFixedPosition = { 240.f - 1024.f, 192.5f - 1024.f };

    mChangeMap("assets/maps/house_interior.json");
}

