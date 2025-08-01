#include "Chest.h"
#include <iostream>

Chest::Chest(const sf::Vector2f& position, float sizeIncrement, const std::string filename,
             const sf::Vector2f& collPosition, const sf::Vector2f& collSize,
             const sf::Vector2f& interactPosition, const sf::Vector2f& interactSize,
             bool& showMenu)
    : Entity(position, filename, collPosition, collSize, 1, interactPosition, interactSize),
    mShowMenu(showMenu) {
    mSprite.setScale(sf::Vector2f(sizeIncrement, sizeIncrement));
}

void Chest::interact() {
    mShowMenu = !mShowMenu;
}

