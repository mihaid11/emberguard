#include "Chest.h"
#include <iostream>

Chest::Chest(const sf::Vector2f& position, float sizeIncrement, const std::string filename,
             const sf::Vector2f& collPosition, const sf::Vector2f& collSize,
             const sf::Vector2f& interactPosition, const sf::Vector2f& interactSize,
             std::function<void(bool)> openCallback)
    : Entity(position, filename, collPosition, collSize, 1, interactPosition, interactSize),
    mOpenCallback(openCallback) {
    mSprite.setScale(sf::Vector2f(sizeIncrement, sizeIncrement));
}

void Chest::interact() {
    mOpenCallback(true);
}
