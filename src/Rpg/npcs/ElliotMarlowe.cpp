#include "ElliotMarlowe.h"
#include <iostream>

ElliotMarlowe::ElliotMarlowe(const sf::Vector2f& position, const std::string& id)
    : NPC(position, id) {
    mPath.push_back(sf::Vector2f(0.0f, 0.0f));
    mPath.push_back(sf::Vector2f(0.0f, 500.0f));
    mPath.push_back(sf::Vector2f(700.0f, 500.0f));
    mPath.push_back(sf::Vector2f(700.0f, 0.0f));
}

