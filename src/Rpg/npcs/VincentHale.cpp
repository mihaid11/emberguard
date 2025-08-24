#include "VincentHale.h"

VincentHale::VincentHale(const sf::Vector2f& position, const std::string& id)
    : NPC(position, id) {
    mPath.push_back(sf::Vector2f(600.0f, 400.0f));
    mPath.push_back(sf::Vector2f(700.0f, 400.0f));
    mPath.push_back(sf::Vector2f(700.0f, 500.0f));
    mPath.push_back(sf::Vector2f(600.0f, 500.0f));
}

