#include "Chest.h"
#include <iostream>

Chest::Chest(const sf::Vector2f& position, const std::string filename,
             const sf::Vector2f& interactPosition, const sf::Vector2f& interactSize,
             std::function<void(bool)> openCallback, const sf::Vector2f& collPosition,
             const sf::Vector2f& collSize)
    : Entity(position, sf::Vector2f(0.f, 0.f), filename, collPosition, collSize, 1, interactPosition, interactSize),
    mOpenCallback(openCallback) {
}

void Chest::interact() {
    mOpenCallback(true);
}
