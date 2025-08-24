#include "MiraStanton.h"

MiraStanton::MiraStanton(const sf::Vector2f& position, const std::string& id)
    : NPC(position, id) {
    mPath.push_back(sf::Vector2f(1000.0f, 900.0f));
    mPath.push_back(sf::Vector2f(700.0f, 900.0f));
    mPath.push_back(sf::Vector2f(700.0f, 1000.0f));
    mPath.push_back(sf::Vector2f(500.0f, 1000.0f));
}

